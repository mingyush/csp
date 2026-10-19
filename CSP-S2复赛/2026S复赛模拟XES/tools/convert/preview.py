# -*- coding: utf-8 -*-
"""把整理好的题面 Markdown 渲染成单页 HTML 预览（KaTeX 公式 + 本地图片）。

用于直观确认「公式 + 图片 + 表格」显示正常，不需要开 OJ。

用法（在 2026S复赛模拟XES/ 下执行）
-----------------------------------
python tools/convert/preview.py --root 模考1
python tools/convert/preview.py --root 模考1 --out 模考1/题面预览.html --title "模考 1"

几点说明
--------
* 题目自动发现：扫描 root 下所有 *.md（跳过 README.md 与 tools/），按 T1..T12 自然序排列。
* 图片路径**不做字符串拼接**，一律用 os.path.relpath 从 md 所在目录算到预览页所在目录，
  这样「预览页在上级目录、图片在题目目录」不会 404（老脚本用手写前缀，正是这里踩过坑）。
* 公式先做 HTML 转义再交给 KaTeX：KaTeX auto-render 读的是 textContent，
  转义后浏览器会把 &lt; 还原成 <，所以 $x < y$ 这类比较符必须转义、不能原样输出。
"""
from __future__ import annotations

import argparse
import html as html_mod
import os
import re
from pathlib import Path

KATEX_CDN = "https://cdn.jsdelivr.net/npm/katex@0.16.9/dist"

TPL = """<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="utf-8">
<title>__TITLE__ 题面预览</title>
<link rel="stylesheet" href="__KATEX__/katex.min.css">
<script defer src="__KATEX__/katex.min.js"></script>
<script defer src="__KATEX__/contrib/auto-render.min.js"
  onload="renderMathInElement(document.body,{delimiters:[{left:'$$',right:'$$',display:true},{left:'$',right:'$',display:false}],throwOnError:false});"></script>
<style>
  :root { color-scheme: light; }
  body { max-width: 860px; margin: 0 auto; padding: 32px 20px 80px;
         font: 16px/1.75 -apple-system, "PingFang SC", "Microsoft YaHei", sans-serif;
         color: #1f2328; background: #fff; }
  h1 { font-size: 24px; border-bottom: 2px solid #d0d7de; padding-bottom: 8px; margin-top: 56px; }
  h1:first-of-type { margin-top: 0; }
  h2 { font-size: 20px; margin-top: 32px; }
  h3 { font-size: 17px; margin-top: 24px; color: #444; }
  h4 { font-size: 16px; margin-top: 20px; color: #444; }
  blockquote { margin: 12px 0; padding: 4px 14px; color: #57606a;
               border-left: 4px solid #d0d7de; background: #f6f8fa; }
  pre.sample { background: #f6f8fa; border: 1px solid #d0d7de; border-radius: 6px;
               padding: 10px 12px; overflow-x: auto; font: 13px/1.5 ui-monospace, Menlo, monospace; }
  img { max-width: 100%; border: 1px solid #e1e4e8; border-radius: 6px; margin: 8px 0; }
  ul, ol { padding-left: 24px; }
  table.tbl { border-collapse: collapse; margin: 12px 0; font-size: 15px; }
  table.tbl th, table.tbl td { border: 1px solid #d0d7de; padding: 6px 14px; text-align: center; }
  table.tbl thead th { background: #f6f8fa; }
  code { background: #f6f8fa; padding: 1px 5px; border-radius: 4px; font-size: 90%; }
  nav { margin: 12px 0 8px; } nav a { margin-right: 14px; }
  .meta { color: #57606a; font-size: 14px; }
</style>
</head>
<body>
<h1>__TITLE__</h1>
<p class="meta">由本地题面 Markdown 渲染 · 公式 KaTeX · 图片取自题目目录 · 共 __COUNT__ 题</p>
<nav>__NAV__</nav>
__CONTENT__
</body>
</html>
"""


# --------------------------------------------------------------------------- #
# 极简 Markdown -> HTML（只覆盖题面实际用到的语法）
# --------------------------------------------------------------------------- #
def inline(s: str) -> str:
    """转义 HTML，再还原 **粗体** / `行内代码` / ![图片](路径)。

    公式里的 < > & 必须转义：KaTeX auto-render 读 textContent，
    浏览器会把 &lt; 还原成 <，所以这里转义是正确行为。
    """
    s = html_mod.escape(s, quote=False)
    s = re.sub(r"`([^`]+)`", r"<code>\1</code>", s)
    s = re.sub(r"\*\*(.+?)\*\*", r"<strong>\1</strong>", s)
    return s


def md_to_html(md_text: str) -> str:
    lines = md_text.split("\n")
    out: list[str] = []
    i = 0
    in_code = False
    in_comment = False
    code_buf: list[str] = []
    para_buf: list[str] = []
    table_buf: list[str] = []
    list_stack: list[tuple[int, str]] = []   # [(缩进宽度, 'ul'|'ol')]

    def flush_para() -> None:
        if para_buf:
            out.append("<p>" + " ".join(inline(x) for x in para_buf) + "</p>")
            para_buf.clear()

    def flush_list() -> None:
        while list_stack:
            _w, tag = list_stack.pop()
            out.append(f"</{tag}>")

    def sync_list(indent: int, tag: str) -> None:
        while list_stack and (list_stack[-1][0] > indent
                              or (list_stack[-1][0] == indent and list_stack[-1][1] != tag)):
            _w, t = list_stack.pop()
            out.append(f"</{t}>")
        if not list_stack or list_stack[-1][0] < indent:
            list_stack.append((indent, tag))
            out.append(f"<{tag}>")

    def flush_table() -> None:
        if not table_buf:
            return

        def cells(row: str) -> list[str]:
            return [c.strip() for c in row.strip().strip("|").split("|")]

        head = cells(table_buf[0])
        body = [cells(r) for r in table_buf[2:]] if len(table_buf) > 1 else []
        h = "".join(f"<th>{inline(c)}</th>" for c in head)
        b = "".join("<tr>" + "".join(f"<td>{inline(c)}</td>" for c in r) + "</tr>" for r in body)
        out.append(f'<table class="tbl"><thead><tr>{h}</tr></thead><tbody>{b}</tbody></table>')
        table_buf.clear()

    while i < len(lines):
        ln = lines[i]

        if in_comment:
            if "-->" in ln:
                in_comment = False
                ln = ln.split("-->", 1)[1]
            else:
                i += 1
                continue

        if ln.startswith("```"):
            flush_para(); flush_list(); flush_table()
            if not in_code:
                in_code, code_buf = True, []
            else:
                out.append('<pre class="sample">' + html_mod.escape("\n".join(code_buf)) + "</pre>")
                in_code = False
            i += 1
            continue
        if in_code:
            code_buf.append(ln)
            i += 1
            continue

        if ln.lstrip().startswith("<!--"):
            flush_para(); flush_list(); flush_table()
            if "-->" not in ln:
                in_comment = True
            i += 1
            continue

        if re.match(r"^\s*\|.*\|\s*$", ln):
            flush_para(); flush_list()
            table_buf.append(ln)
            i += 1
            continue
        flush_table()

        m_li = re.match(r"^( *)([-*]|\d+\.)\s+(.*)$", ln)
        m_hd = re.match(r"^(#+)\s*(.*)$", ln)
        if m_hd:
            flush_para(); flush_list()
            lvl = min(len(m_hd.group(1)) + 1, 4)   # 题面 h1 -> 预览 h2
            out.append(f"<h{lvl}>{inline(m_hd.group(2))}</h{lvl}>")
        elif ln.startswith("> "):
            flush_para(); flush_list()
            out.append(f"<blockquote>{inline(ln[2:])}</blockquote>")
        elif m_li:
            flush_para()
            tag = "ol" if m_li.group(2)[0].isdigit() else "ul"
            sync_list(len(m_li.group(1)), tag)
            out.append(f"<li>{inline(m_li.group(3))}</li>")
        elif not ln.strip():
            flush_para(); flush_list()
        else:
            flush_list()
            para_buf.append(ln.strip())
        i += 1

    flush_para(); flush_list(); flush_table()
    return "\n".join(out)


def md_to_html_with_images(md_text: str, img_resolver) -> str:
    """先把 ![](src) 抽成占位符（避免被转义/正则二次处理），渲染完再换回 <img>。"""
    holders: dict[str, str] = {}

    def stash(m: re.Match) -> str:
        key = f"\x00I{len(holders)}\x00"
        holders[key] = img_resolver(m.group(2), m.group(1))
        return key

    prepared = re.sub(r"!\[([^\]]*)\]\(([^)\n]+)\)", stash, md_text)
    html = md_to_html(prepared)
    for key, tag in holders.items():
        html = html.replace(key, tag)
    return html


# --------------------------------------------------------------------------- #
def natkey(s: str):
    return [int(t) if t.isdigit() else t.lower() for t in re.split(r"(\d+)", str(s))]


def discover(root: Path, exclude: set[str]) -> list[Path]:
    mds = [p for p in root.rglob("*.md")
           if p.name not in exclude and "tools" not in p.parts and not p.name.startswith(".")]
    return sorted(mds, key=lambda p: natkey(str(p.relative_to(root))))


def render(root: Path, out: Path, title: str, exclude: set[str], katex: str) -> int:
    mds = discover(root, exclude)
    if not mds:
        print(f"{root} 下没找到题面 md")
        return 2
    out_dir = out.parent

    navs, body, n_formula = [], [], 0
    for idx, md in enumerate(mds, 1):
        md_dir = md.parent
        text = md.read_text(encoding="utf-8")
        n_formula += len(re.findall(r"\$\$[^$]+\$\$|\$[^$\n]+\$", text))
        head = next((ln for ln in text.split("\n") if ln.startswith("# ")), "")
        label = head.lstrip("# ").strip() or md.stem
        key = md_dir.name if md_dir != root else md.stem
        anchor = f"p{idx}-{key}"

        def resolver(src: str, alt: str, _md_dir=md_dir) -> str:
            rel = os.path.relpath(_md_dir / src, out_dir)
            return f'<img alt="{html_mod.escape(alt, quote=True)}" src="{rel.replace(os.sep, "/")}">'

        navs.append(f'<a href="#{anchor}">{key} {label}</a>')
        body.append(f'<section id="{anchor}">')
        body.append(md_to_html_with_images(text, resolver))
        body.append("</section>")

    doc = (TPL.replace("__TITLE__", html_mod.escape(title))
              .replace("__KATEX__", katex)
              .replace("__COUNT__", str(len(mds)))
              .replace("__NAV__", " · ".join(navs))
              .replace("__CONTENT__", "\n".join(body)))
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(doc, encoding="utf-8")
    print(f"-> {out}  ({len(doc)}B, {len(mds)} 题, {n_formula} 个公式)")
    return 0


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description="题面 Markdown -> 单页 HTML 预览")
    ap.add_argument("--root", required=True, help="题目根目录（如 模考1）")
    ap.add_argument("--out", help="输出 html，默认 <root>/题面预览.html")
    ap.add_argument("--title", help="预览页标题，默认取 root 目录名")
    ap.add_argument("--exclude", default="README.md", help="跳过的文件名，逗号分隔")
    ap.add_argument("--katex", default=KATEX_CDN, help="KaTeX CDN 前缀（离线可换成本地路径）")
    args = ap.parse_args(argv)

    root = Path(args.root).resolve()
    if not root.is_dir():
        print(f"目录不存在：{root}")
        return 2
    out = Path(args.out).resolve() if args.out else root / "题面预览.html"
    exclude = {x.strip() for x in args.exclude.split(",") if x.strip()}
    return render(root, out, args.title or root.name, exclude, args.katex.rstrip("/"))


if __name__ == "__main__":
    raise SystemExit(main())
