# -*- coding: utf-8 -*-
"""小猴编程（xiaohoucode）OJ 题面转换：raw HTML 快照 -> 干净、可渲染、自包含的 Markdown。

子命令
------
parse   扫描 raw HTML -> 生成题面 Markdown + 图片落盘 + images.json（待转写清单）
inject  把 images.json 里填好的 content 回填进 Markdown（raw 已丢失也能用）
images  汇总打印待转写图片清单，供人工 / 模型逐张转写

常用用法（在 2026S复赛模拟XES/ 下执行）
--------------------------------------
python tools/convert/convert.py parse  --raw 模考2/_raw
python tools/convert/convert.py images --root 模考2 --format md
python tools/convert/convert.py inject --root 模考2
python tools/convert/preview.py        --root 模考2

设计原则
--------
1. 解析规则来自真实页面（见 README「DOM 坑位」），不做猜想式改写。
   要改行为，先补 tests/fixture_xiaohou.html，再改代码——raw 快照会丢，测试不能丢。
2. 图片最终都要变成 Markdown 文本（题面不依赖图片显示）：
   parse 出清单 -> 人 / 模型填 content -> inject 回填。图片本身只作原始素材留档。
3. 重跑安全：images.json 按图片 URL 合并，已有的 file / content / status 不被覆盖。
"""
from __future__ import annotations

import argparse
import json
import re
import sys
import urllib.parse
import urllib.request
from pathlib import Path

try:
    from bs4 import BeautifulSoup, Comment, NavigableString
except ImportError:  # pragma: no cover
    sys.exit("需要 beautifulsoup4 + lxml：pip install beautifulsoup4 lxml")

UA = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120 Safari/537.36"

IMG_EXTS = {".jpeg", ".jpg", ".png", ".gif", ".webp", ".bmp", ".svg"}
CT_EXTS = {
    "image/jpeg": ".jpeg", "image/jpg": ".jpeg", "image/png": ".png",
    "image/gif": ".gif", "image/webp": ".webp", "image/bmp": ".bmp",
    "image/svg+xml": ".svg",
}

HEADING_MAP = {
    "题目描述": "题目描述",
    "题目背景": "题目背景",
    "【输入格式】": "输入格式",
    "输入格式": "输入格式",
    "【输出格式】": "输出格式",
    "输出格式": "输出格式",
    "【说明提示】": "说明提示",
    "【说明/提示】": "说明提示",
    "说明提示": "说明提示",
    "【数据范围】": "数据范围",
    "数据范围": "数据范围",
    "【提示】": "提示",
    "提示": "提示",
    "【样例解释】": "样例解释",
}


# --------------------------------------------------------------------------- #
# 渲染上下文：跟踪当前小节 + 收集图片
# --------------------------------------------------------------------------- #
class Ctx:
    def __init__(self) -> None:
        self.section = ""
        self.images: list[dict] = []
        self._by_src: dict[str, dict] = {}

    def register(self, src: str, alt: str | None) -> str:
        """登记一张图片，返回占位符。同一 src 只登记一次。"""
        hit = self._by_src.get(src)
        if hit is not None:
            return hit["_token"]
        idx = len(self.images) + 1
        suf = Path(urllib.parse.urlparse(src).path).suffix.lower()
        no_ext = suf not in IMG_EXTS
        entry = {
            "id": f"img{idx}",
            "file": f"img{idx}{suf if not no_ext else '.png'}",
            "src": src,
            "alt": alt or "图片",
            "section": self.section,
            "context": "",
            "content": "",
            "status": "pending",
            "_token": f"\x00IMG{idx}\x00",
            "_no_ext": no_ext,
        }
        self.images.append(entry)
        self._by_src[src] = entry
        return entry["_token"]


# --------------------------------------------------------------------------- #
# HTML 片段 -> Markdown 片段
# --------------------------------------------------------------------------- #
def heading_text(raw: str) -> str:
    raw = raw.strip()
    if raw in HEADING_MAP:
        return HEADING_MAP[raw]
    m = re.fullmatch(r"【?输入输出样例\s*#?\s*(\d+)】?", raw)
    if m:
        return f"样例 #{m.group(1)}"
    return raw.strip("【】")


def clean(s: str) -> str:
    s = s.replace("\u00a0", " ")
    s = re.sub(r"[ \t]+", " ", s)
    s = re.sub(r" *\n *", "\n", s)
    s = re.sub(r"\n{3,}", "\n\n", s)
    return s.strip()


def render_node(node, ctx: Ctx) -> str:
    if isinstance(node, Comment):
        return ""          # Comment 是 NavigableString 子类，不挡掉会漏进正文
    if isinstance(node, NavigableString):
        return str(node)

    cls = node.get("class") or []
    name = node.name

    if "katex" in cls:
        ann = node.find("annotation", attrs={"encoding": "application/x-tex"})
        tex = (ann.get_text() if ann else node.get_text()).strip()
        parent_cls = (node.parent.get("class") or []) if node.parent else []
        return f"$${tex}$$" if "katex-display" in parent_cls else f"${tex}$"
    if name == "img":
        src = node.get("src") or node.get("data-src") or ""
        if not src:
            return ""
        return ctx.register(src, node.get("alt"))
    if name == "br":
        return "\n"
    if name == "strong" or name == "b":
        return f"**{render_children(node, ctx)}**"
    if name in ("em", "i"):
        return f"*{render_children(node, ctx)}*"
    if name == "code":
        return f"`{node.get_text()}`"
    if name == "sup":
        return f"^{render_children(node, ctx)}"
    if name == "sub":
        return f"~{render_children(node, ctx)}"
    if name in ("katex-html", "katex-mathml", "script", "style"):
        return ""
    if name == "a":
        href = (node.get("href") or "").strip()
        text = clean(render_children(node, ctx))
        if href and (href.startswith("http") or href.startswith("www")):
            return f"[{text or href}]({href})"
        if href and not href.startswith("#"):
            return f"[{text or href}]({href})"
        return text
    return render_children(node, ctx)


def render_children(node, ctx: Ctx) -> str:
    return "".join(render_node(c, ctx) for c in node.children)


def render_list(node, ctx: Ctx, indent: int = 0) -> str:
    """列表 -> Markdown，支持一层嵌套。"""
    ordered = node.name == "ol"
    pad = "  " * indent
    out: list[str] = []
    for i, li in enumerate(node.find_all("li", recursive=False), 1):
        bullet = f"{i}." if ordered else "-"
        nested = li.find(["ul", "ol"], recursive=False)
        if nested is not None:
            head = "".join(
                render_node(c, ctx) for c in li.children if c is not nested
            )
            out.append(f"{pad}{bullet} {clean(head)}")
            out.append(render_list(nested, ctx, indent + 1))
        else:
            out.append(f"{pad}{bullet} {clean(render_children(li, ctx))}")
    return "\n".join(x for x in out if x.strip())


def render_table(node, ctx: Ctx) -> str:
    """HTML 表格 -> Markdown 表格（题面里的数据范围表多为图片，此处兜底）。"""
    rows: list[list[str]] = []
    for tr in node.find_all("tr"):
        cells = tr.find_all(["th", "td"], recursive=False) or tr.find_all(["th", "td"])
        if not cells:
            continue
        rows.append([clean(render_children(c, ctx)).replace("\n", " ") or " " for c in cells])
    if not rows:
        return ""
    width = max(len(r) for r in rows)
    rows = [r + [" "] * (width - len(r)) for r in rows]
    head, body = rows[0], rows[1:]
    lines = ["| " + " | ".join(head) + " |", "| " + " | ".join(["---"] * width) + " |"]
    lines += ["| " + " | ".join(r) + " |" for r in body]
    return "\n".join(lines)


def to_blocks(children, ctx: Ctx) -> list[tuple[str, str]]:
    """把一段子节点序列拆成块：('p'|'list'|'table'|'math', text)。"""
    blocks: list[tuple[str, str]] = []
    buf: list[str] = []

    def flush() -> None:
        text = clean("".join(buf))
        if text:
            if text.startswith("$$") and text.endswith("$$") and text.count("$$") == 2:
                blocks.append(("math", text))
            else:
                blocks.append(("p", text))
        buf.clear()

    for child in children:
        if isinstance(child, Comment):
            continue
        if isinstance(child, NavigableString):
            buf.append(str(child))
            continue
        name = child.name
        if name in ("ul", "ol"):
            flush()
            blocks.append(("list", render_list(child, ctx)))
            continue
        if name == "table":
            flush()
            blocks.append(("table", render_table(child, ctx)))
            continue
        if name == "div":
            if "sub-title" in (child.get("class") or []):
                continue
            flush()
            blocks.extend(to_blocks(child.children, ctx))
            continue
        if name == "br":
            buf.append("\n")
            continue
        buf.append(render_node(child, ctx))
    flush()
    return blocks


def filter_blocks(blocks, seen: set[str], stats: dict) -> list[tuple[str, str]]:
    """同一小节内，去掉重复的长段落（原页面 div 未闭合会导致整段重复）。"""
    out = []
    for kind, text in blocks:
        if kind == "p" and len(text) >= 15:
            key = re.sub(r"\s+", "", text)
            if key in seen:
                stats["dedup"] += 1
                continue
            seen.add(key)
        out.append((kind, text))
    return out


def blocks_to_lines(blocks) -> list[str]:
    lines: list[str] = []
    for i, (kind, text) in enumerate(blocks):
        if not text:
            continue
        if kind == "math":
            lines += ["", text, ""]
        elif kind == "list":
            if i > 0 and blocks[i - 1][0] == "p":
                lines.append("")
            lines += [text, ""]
        else:
            if i > 0 and blocks[i - 1][0] == "list":
                lines.append("")
            lines += [text, ""]
    return lines


# --------------------------------------------------------------------------- #
# 图片落盘 + images.json
# --------------------------------------------------------------------------- #
def fetch(url: str) -> tuple[bytes, str]:
    req = urllib.request.Request(url, headers={"User-Agent": UA})
    with urllib.request.urlopen(req, timeout=60) as resp:
        return resp.read(), resp.headers.get("Content-Type", "")


def localize_images(ctx: Ctx, problem_dir: Path, img_dir_rel: str, download: bool,
                    existing: dict[str, dict]) -> None:
    """图片落盘（可选）并合并已有的 file / content / status。"""
    out_dir = problem_dir / img_dir_rel if img_dir_rel else problem_dir
    for e in ctx.images:
        old = existing.get(e["src"]) or {}
        if old.get("file"):
            e["file"] = old["file"]
        if old.get("content"):
            e["content"] = old["content"]
        e["status"] = old.get("status") or ("transcribed" if e["content"] else "pending")
        if old.get("alt"):
            e["alt"] = old["alt"]

        dst = out_dir / e["file"]
        if dst.exists() or not download:
            continue
        out_dir.mkdir(parents=True, exist_ok=True)
        try:
            data, ctype = fetch(e["src"])
        except Exception as exc:  # 网络失败不应中断整篇转换
            e["status"] = "download-failed"
            e["note"] = f"{type(exc).__name__}: {exc}"
            print(f"   ! 图片下载失败 {e['src']}: {exc}")
            continue
        ext = CT_EXTS.get((ctype or "").split(";")[0].strip().lower())
        if ext and e["_no_ext"] and Path(e["file"]).suffix.lower() != ext:
            e["file"] = Path(e["file"]).with_suffix(ext).name
            dst = out_dir / e["file"]
        dst.write_bytes(data)


def img_ref(e: dict, img_dir_rel: str) -> str:
    prefix = f"{img_dir_rel}/" if img_dir_rel else ""
    return f"![{e['alt']}]({prefix}{e['file']})"


def dump_images_json(problem_dir: Path, problem: str, raw_name: str | None,
                     images: list[dict]) -> Path:
    payload = {
        "_comment": "图片转写清单：把图片内容填进 content（Markdown 片段），"
                    "再跑 convert.py inject 回填题面。status 会自动更新。",
        "problem": problem,
        "source_html": raw_name,
        "images": [{k: v for k, v in e.items() if not k.startswith("_")} for e in images],
    }
    dst = problem_dir / "images.json"
    dst.write_text(json.dumps(payload, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    return dst


def load_images_json(problem_dir: Path) -> tuple[dict[str, dict], dict]:
    p = problem_dir / "images.json"
    if not p.exists():
        return {}, {}
    try:
        data = json.loads(p.read_text(encoding="utf-8"))
    except json.JSONDecodeError as exc:
        print(f"   ! images.json 解析失败（{exc}），按无清单处理")
        return {}, {}
    return {e.get("src"): e for e in data.get("images", []) if e.get("src")}, data


def apply_content(text: str, images: list[dict], img_dir_rel: str) -> tuple[str, list[str], list[str]]:
    """把有 content 的图片引用替换成 content。

    返回 (新文本, 已回填文件列表, 有 content 但题面里找不到引用的图片 id 列表)。
    幂等：已经回填过的题面里没有图片引用了，此时保持 status=transcribed，不降级、不重复追加。
    """
    done: list[str] = []
    missing: list[str] = []
    for e in images:
        if not e.get("content"):
            continue
        ref = re.escape(img_ref(e, img_dir_rel))
        block = e["content"].strip()
        new, n = re.subn(ref, lambda m: "\n\n" + block + "\n\n", text, count=1)
        if not n:
            new, n = re.subn(
                r"!\[[^\]]*\]\(" + re.escape(e["file"]) + r"\)",
                lambda m: "\n\n" + block + "\n\n", text, count=1)
        if n:
            text = new
            done.append(e["file"])
            e["status"] = "transcribed"
        elif e.get("status") != "transcribed":
            e["status"] = "content-without-ref"
            missing.append(e["id"])
    return text, done, missing


def add_footnotes(text: str, images: list[dict], img_dir_rel: str, done: list[str]) -> str:
    prefix = f"{img_dir_rel}/" if img_dir_rel else ""
    lines = []
    for e in images:
        if e["file"] in done:
            lines.append(
                f"<!-- 图片原图：{prefix}{e['file']}"
                + (f"（来源 {e['src']}）" if e.get("src") else "")
                + "，内容已转写为上方 Markdown，原图仅作素材留档 -->")
    if not lines:
        return text
    kept = [ln for ln in text.rstrip().split("\n")]
    kept = [ln for ln in kept if not ln.startswith("<!-- 图片原图：")]
    while kept and not kept[-1].strip():
        kept.pop()
    return "\n".join(kept) + "\n\n" + "\n".join(lines) + "\n"


# --------------------------------------------------------------------------- #
# parse：raw HTML -> Markdown
# --------------------------------------------------------------------------- #
KEY_RE = re.compile(r"^([TtPpAa]\d+|\d+)\s*[-_　 ]?\s*(.*)$")
SKIP_RAW_NAMES = {"readme", "index"}


def discover_jobs(raw: Path) -> list[Path]:
    if raw.is_file():
        return [raw]
    files = [p for p in sorted(raw.iterdir())
             if p.suffix.lower() in (".html", ".htm") and p.stem.lower() not in SKIP_RAW_NAMES]
    return files


def job_key(stem: str) -> tuple[str, str]:
    """从 raw 文件名拆出 (题号, 题名线索)。T1配对系列 -> ('T1', '配对系列')。"""
    m = KEY_RE.match(stem)
    if m:
        return m.group(1).upper(), m.group(2).strip()
    return "", stem.strip()


def parse_one(raw: Path, out_root: Path, img_dir_rel: str, download: bool) -> dict:
    key, _hint = job_key(raw.stem)
    problem_dir = out_root / key if key else out_root
    problem_dir.mkdir(parents=True, exist_ok=True)

    soup = BeautifulSoup(raw.read_text(encoding="utf-8"), "lxml")
    main = soup.select_one(".main") or soup

    title_el = soup.select_one(".top__item__name")
    title = title_el.get_text(strip=True) if title_el else (key or raw.stem)

    ctx = Ctx()
    lines: list[str] = [f"# {title}", ""]

    tags: list[str] = []
    for t in soup.select(".top__item__tag"):
        txt = t.get_text(strip=True)
        if txt and txt not in tags:
            tags.append(txt)
    if tags:
        lines += ["> " + "　".join(tags), ""]

    stats = {"dedup": 0, "samples": 0, "partial": 0}
    seen: set[str] = set()

    def emit(blocks) -> None:
        lines.extend(blocks_to_lines(filter_blocks(blocks, seen, stats)))

    for node in main.find_all("div"):
        # 样例区 / 隐藏副本内部的一切都由父级统一处理
        if node.find_parent(class_="sample__list") or node.find_parent(class_="desc-html"):
            continue
        # .desc 内部（含 desc__text）交给最外层 .desc 按 DOM 顺序整体处理，避免乱序或重复
        if node.find_parent(class_="desc"):
            continue
        cls = node.get("class") or []

        if "sample__list" in cls:
            lines += ["## 输入输出样例", ""]
            partial: list[tuple[str, str]] = []
            for item in node.select(".sample__list__item"):
                st = item.select_one(".sub-title")
                label = heading_text(st.get_text(strip=True)) if st else "样例"
                blobs = [pre.get_text().strip("\n") if (pre := b.select_one("pre")) else ""
                         for b in item.select(".main__example__item")]
                if blobs and all("OI 赛制下" in bl or "该组数据" in bl for bl in blobs):
                    note = re.sub(r"\n?注意：OI 赛制下.*$", "", blobs[0], flags=re.S).strip()
                    partial.append((label, note))
                    continue
                stats["samples"] += 1
                lines += [f"### {label}", ""]
                for b in item.select(".main__example__item"):
                    label_div = b.find("div", recursive=False)
                    l2 = label_div.get_text(strip=True) if label_div else ""
                    pre = b.select_one("pre")
                    content = pre.get_text().strip("\n") if pre else ""
                    lines += [f"**{l2}**", "", "```text", content, "```", ""]
            if partial:
                stats["partial"] = len(partial)
                lines += ["### 部分分测试点（无样例）", "",
                          "> OI 赛制下评测只显示该测试点是否通过。", ""]
                lines += [f"- **{label}**：{note}" for label, note in partial]
                lines.append("")

        elif "sub-title" in cls:
            head = heading_text(node.get_text(strip=True))
            if head:
                ctx.section = head
                seen = set()
                lines += [f"## {head}", ""]

        elif "desc__text" in cls:
            emit(to_blocks(node.children, ctx))

        elif "desc" in cls:
            emit(to_blocks(node.children, ctx))

        elif "practice__desc" in cls:
            has_sub = node.find("div", class_="sub-title", recursive=False) is not None
            has_desc = node.find("div", class_="desc", recursive=False) is not None
            if not has_sub and not has_desc:
                emit(to_blocks(node.children, ctx))

    text = re.sub(r"\n{3,}", "\n\n", "\n".join(lines)).strip() + "\n"

    # 图片落盘 + 回填已有转写内容
    existing, _ = load_images_json(problem_dir)
    localize_images(ctx, problem_dir, img_dir_rel, download, existing)
    for e in ctx.images:
        text = text.replace(e["_token"], img_ref(e, img_dir_rel))
    text, done, _missing = apply_content(text, ctx.images, img_dir_rel)
    text = re.sub(r"\n{3,}", "\n\n", text).rstrip() + "\n"
    text = add_footnotes(text, ctx.images, img_dir_rel, done)

    # 待转写图片的上下文（给转写者看的定位线索）
    for e in ctx.images:
        if e.get("context"):
            continue
        e["context"] = image_context(text, img_ref(e, img_dir_rel)) or e["section"]

    md = problem_dir / f"{raw.stem}.md"
    md.write_text(text, encoding="utf-8")
    js = dump_images_json(problem_dir, title, raw.name, ctx.images)

    formulas = len(re.findall(r"\$[^$\n]+\$|\$\$[^$]+\$\$", text))
    return {
        "key": key or raw.stem, "title": title, "md": md, "raw": raw.name,
        "images": len(ctx.images), "transcribed": len(done),
        "formulas": formulas, "stats": stats, "json": js,
    }


def image_context(text: str, ref: str) -> str:
    """图片所在位置的上下文（就近取一行正文），给转写者定位用。"""
    lines = text.split("\n")

    def usable(ln: str) -> bool:
        s = ln.strip()
        return bool(s) and not s.startswith(("<!--", "#", "![", "|", "```"))

    for i, ln in enumerate(lines):
        if ref not in ln:
            continue
        for j in range(i - 1, -1, -1):
            if usable(lines[j]):
                s = re.sub(r"\s+", " ", lines[j].strip())
                return s[:80] + ("…" if len(s) > 80 else "")
        for j in range(i + 1, len(lines)):
            if usable(lines[j]):
                s = re.sub(r"\s+", " ", lines[j].strip())
                return s[:80] + ("…" if len(s) > 80 else "")
    return ""


def cmd_parse(args) -> int:
    raw = Path(args.raw).resolve()
    if not raw.exists():
        print(f"raw 不存在：{raw}")
        return 2
    if args.out:
        out_root = Path(args.out).resolve()
    else:
        # 约定：raw 目录名是 _raw 时，输出到它的上一级
        out_root = raw.parent if raw.is_dir() and raw.name.startswith("_") else (
            raw.parent if raw.is_file() else raw)
    jobs = discover_jobs(raw)
    if args.only:
        want = {x.strip().upper() for x in args.only.split(",")}
        jobs = [j for j in jobs if job_key(j.stem)[0] in want or j.stem.upper() in want]
    if not jobs:
        print(f"没有找到 raw HTML：{raw}")
        return 2

    print(f"raw  : {raw}")
    print(f"out  : {out_root}")
    rows = []
    for j in jobs:
        info = parse_one(j, out_root, args.img_dir, download=not args.no_download)
        rows.append(info)
        st = info["stats"]
        print(f"  {info['key']:<6} {info['title']:<12} 公式 {info['formulas']:>4}  "
              f"图片 {info['images']}（已转写 {info['transcribed']}）  "
              f"样例 {st['samples']}  去重 {st['dedup']}")
    print(f"\n产出 {len(rows)} 篇：")
    for r in rows:
        print(f"  {r['md']}")
    pending = sum(r["images"] - r["transcribed"] for r in rows)
    if pending:
        print(f"\n还有 {pending} 张图片待转写："
              f"\n  python tools/convert/convert.py images --root {out_root} --format md")
    return 0


# --------------------------------------------------------------------------- #
# inject：images.json 的 content 回填题面
# --------------------------------------------------------------------------- #
def find_problem_dirs(root: Path) -> list[Path]:
    return sorted({p.parent for p in root.rglob("images.json")})


def cmd_inject(args) -> int:
    roots = [Path(x).resolve() for x in args.root]
    dirs: list[Path] = []
    for r in roots:
        dirs += [r] if (r / "images.json").exists() else find_problem_dirs(r)
    if not dirs:
        print("没找到 images.json")
        return 2

    changed = 0
    for d in sorted(set(dirs)):
        existing, payload = load_images_json(d)
        images = payload.get("images", [])
        pending = [e for e in images if not e.get("content")]
        if not any(e.get("content") for e in images):
            print(f"  {d.name}: 无 content 可回填（{len(pending)} 张待转写）")
            continue
        for md in sorted(d.glob("*.md")):
            text = md.read_text(encoding="utf-8")
            new, done, missing = apply_content(text, images, args.img_dir)
            if missing:
                print(f"     ! {md.name}: {', '.join(missing)} 有 content 但题面里找不到对应图片引用")
            new = re.sub(r"\n{3,}", "\n\n", new)
            new = add_footnotes(new, images, args.img_dir, done)
            if new != text:
                md.write_text(new, encoding="utf-8")
                changed += 1
                print(f"  {md.name}: 回填 {len(done)} 张图片")
        dump_images_json(d, payload.get("problem", d.name), payload.get("source_html"), images)
    print(f"\n更新 {changed} 个题面文件。")
    return 0


# --------------------------------------------------------------------------- #
# images：待转写清单
# --------------------------------------------------------------------------- #
def cmd_images(args) -> int:
    root = Path(args.root).resolve()
    dirs = find_problem_dirs(root)
    rows = []
    for d in dirs:
        _existing, payload = load_images_json(d)
        for e in payload.get("images", []):
            if e.get("content"):
                continue
            rows.append((payload.get("problem") or d.name, d, e))
    if not rows:
        print(f"{root} 下没有待转写图片")
        return 0
    if args.format == "md":
        print("| # | 题目 | 图片文件 | 所在小节 | 上下文 | 图片地址 |")
        print("| --- | --- | --- | --- | --- | --- |")
        for i, (problem, _d, e) in enumerate(rows, 1):
            print(f"| {i} | {problem} | `{e['file']}` | {e.get('section', '')} "
                  f"| {e.get('context', '')} | {e.get('src', '')} |")
    else:
        for i, (problem, d, e) in enumerate(rows, 1):
            print(f"[{i}] {problem} / {e['file']}  小节={e.get('section', '')}")
            print(f"    上下文：{e.get('context', '')}")
            print(f"    地址  ：{e.get('src', '')}")
            print(f"    填进  ：{d / 'images.json'} 的 content 字段，再跑 convert.py inject")
    print(f"\n共 {len(rows)} 张待转写。", file=sys.stderr)
    return 0


# --------------------------------------------------------------------------- #
def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(
        description="小猴编程 OJ 题面转换：HTML 快照 -> Markdown（图片转文本闭环）",
        formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)

    p = sub.add_parser("parse", help="raw HTML -> 题面 Markdown + 图片 + images.json")
    p.add_argument("--raw", required=True, help="raw HTML 文件或目录（约定目录名 _raw）")
    p.add_argument("--out", help="输出根目录，默认取 raw 的上一级")
    p.add_argument("--img-dir", default="", help="图片相对题目目录的子目录，默认与 md 同目录")
    p.add_argument("--only", help="只处理指定题号，逗号分隔，如 T1,T3")
    p.add_argument("--no-download", action="store_true", help="只登记图片，不下载")
    p.set_defaults(func=cmd_parse)

    p = sub.add_parser("inject", help="把 images.json 的 content 回填进题面")
    p.add_argument("--root", nargs="+", required=True, help="题目目录或包含题目目录的根")
    p.add_argument("--img-dir", default="", help="与 parse 时的 --img-dir 保持一致")
    p.set_defaults(func=cmd_inject)

    p = sub.add_parser("images", help="列出待转写图片")
    p.add_argument("--root", required=True)
    p.add_argument("--format", choices=["text", "md"], default="text")
    p.set_defaults(func=cmd_images)

    args = ap.parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    raise SystemExit(main())
