# -*- coding: utf-8 -*-
"""题面 Markdown 交付前自检：公式、代码围栏、链接与图片引用。

用法（在 2026S复赛模拟XES/ 下执行）
-----------------------------------
python tools/convert/check.py --root 模考1
python tools/convert/check.py --root 模考1 --all          # 连 README.md 一起查
python tools/convert/check.py --root 模考1 --node /path/to/node --node-path /path/to/node_modules

检查项
------
1. 代码围栏 ``` 是否成对
2. 公式定界符：$...$ 与 $$...$$ 是否配对（残留裸 $ 会报行号）
3. KaTeX 能否渲染（本地 node + katex；探测不到则降级跳过，不算失败）
4. Markdown 链接 / 图片目标在磁盘上是否存在（含中文与空格路径，做 URL 解码）

退出码：0 全部通过；1 有问题。
"""
from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import urllib.parse
from pathlib import Path

KATEX_JS = r"""
const katex = require('katex');
let buf = '';
process.stdin.setEncoding('utf8');
process.stdin.on('data', d => buf += d);
process.stdin.on('end', () => {
  const items = JSON.parse(buf);
  const out = items.map(it => {
    try {
      katex.renderToString(it.tex, { throwOnError: true, displayMode: !!it.display, strict: false });
      return null;
    } catch (e) {
      return String((e && e.message) || e);
    }
  });
  process.stdout.write(JSON.stringify(out));
});
"""


def find_node(explicit: str | None = None) -> str | None:
    for c in (explicit, os.environ.get("CONVERT_NODE")):
        if c and Path(c).exists():
            return c
    hit = shutil.which("node")
    if hit:
        return hit
    cands = sorted(Path.home().glob(".workbuddy/binaries/node/versions/*/bin/node"))
    return str(cands[-1]) if cands else None


def find_node_path(explicit: str | None = None) -> str | None:
    cands: list[Path] = []
    if explicit:
        cands.append(Path(explicit))
    if os.environ.get("NODE_PATH"):
        cands.append(Path(os.environ["NODE_PATH"]))
    cands.append(Path.home() / ".workbuddy/binaries/node/workspace/node_modules")
    for c in cands:
        if (c / "katex" / "package.json").exists():
            return str(c)
    return None


def katex_version(node_path: str) -> str:
    try:
        pkg = json.loads((Path(node_path) / "katex" / "package.json").read_text(encoding="utf-8"))
        return pkg.get("version", "?")
    except Exception:
        return "?"


def strip_fences(text: str) -> str:
    return re.sub(r"```.*?```", "", text, flags=re.S)


def extract_math(text: str) -> tuple[list[tuple[str, bool]], str]:
    """返回 ([(tex, is_display)], 去掉公式后的残留文本)。"""
    body = strip_fences(text).replace("\\$", "")
    disp = re.findall(r"\$\$(.+?)\$\$", body, flags=re.S)
    rest = re.sub(r"\$\$.+?\$\$", "", body, flags=re.S)
    inl = re.findall(r"\$([^$\n]+?)\$", rest)
    rest = re.sub(r"\$[^$\n]+?\$", "", rest)
    return ([(d.strip(), True) for d in disp] + [(t.strip(), False) for t in inl]), rest


def check_file(md: Path, root: Path) -> dict:
    text = md.read_text(encoding="utf-8")
    issues: list[str] = []

    if is_fence_odd(text):
        issues.append("代码围栏 ``` 不成对")

    math, rest = extract_math(text)
    stray = []
    for i, ln in enumerate(rest.split("\n"), 1):
        if "$" in ln:
            stray.append(i)
    if stray:
        issues.append("残留未配对 $（行 " + ", ".join(map(str, stray[:6])) +
                      ("…" if len(stray) > 6 else "") + "）")

    n_bad = 0
    for i, ln in enumerate(text.split("\n"), 1):
        if ln != ln.rstrip():
            n_bad += 1
    if n_bad:
        issues.append(f"{n_bad} 行有行尾空白")

    links = re.findall(r"(?<!!)\[[^\]]*\]\(([^)\n]+)\)", strip_fences(text))
    imgs = re.findall(r"!\[[^\]]*\]\(([^)\n]+)\)", strip_fences(text))
    broken: list[str] = []
    for target in links + imgs:
        if re.match(r"^(https?:|mailto:|tel:|#)", target):
            continue
        path = target.split("#", 1)[0]
        if not path:
            continue
        p = (md.parent / urllib.parse.unquote(path)).resolve()
        if not p.exists():
            broken.append(target)
    if broken:
        issues.append("链接/图片目标不存在：" + ", ".join(broken[:5]) +
                      ("…" if len(broken) > 5 else ""))

    return {"md": md, "rel": str(md.relative_to(root)) if md.is_relative_to(root) else str(md),
            "math": math, "n_disp": sum(1 for _t, d in math if d), "issues": issues,
            "links": len(links), "imgs": len(imgs)}


def is_fence_odd(text: str) -> bool:
    return len(re.findall(r"^```", text, flags=re.M)) % 2 == 1


def verify_katex(results: list[dict], node: str, node_path: str) -> tuple[dict[tuple[str, str], str], str]:
    """批量校验公式，返回 ({(相对路径, tex): 错误}, 致命错误说明)。"""
    uniq: dict[str, tuple[str, bool]] = {}
    for r in results:
        for tex, disp in r["math"]:
            uniq.setdefault(tex, (r["rel"], disp))
    if not uniq:
        return {}, ""
    items = [{"tex": t, "display": d} for t, (_rel, d) in uniq.items()]
    with tempfile.TemporaryDirectory() as td:
        js = Path(td) / "katex_check.js"
        js.write_text(KATEX_JS, encoding="utf-8")
        env = dict(os.environ, NODE_PATH=node_path)
        try:
            proc = subprocess.run([node, str(js)], input=json.dumps(items), env=env,
                                  capture_output=True, text=True, timeout=180)
        except subprocess.TimeoutExpired:
            return {}, "KaTeX 校验超时"
    if proc.returncode != 0:
        return {}, f"node 退出码 {proc.returncode}: {proc.stderr.strip()[:200]}"
    try:
        errs = json.loads(proc.stdout)
    except json.JSONDecodeError:
        return {}, f"node 输出无法解析: {proc.stdout[:200]}"
    out: dict[tuple[str, str], str] = {}
    for (tex, (rel, _d)), err in zip(uniq.items(), errs):
        if err:
            out[(rel, tex)] = err
    return out, ""


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description="题面 Markdown 自检")
    ap.add_argument("--root", required=True)
    ap.add_argument("--all", action="store_true", help="连 README.md 一起查")
    ap.add_argument("--no-katex", action="store_true", help="跳过 KaTeX 校验")
    ap.add_argument("--node")
    ap.add_argument("--node-path")
    args = ap.parse_args(argv)

    root = Path(args.root).resolve()
    if not root.is_dir():
        print(f"目录不存在：{root}")
        return 2

    mds = sorted(p for p in root.rglob("*.md")
                 if "tools" not in p.parts and (args.all or p.name != "README.md"))
    if not mds:
        print(f"{root} 下没找到 md")
        return 2

    results = [check_file(md, root) for md in mds]

    katex_errs: dict[tuple[str, str], str] = {}
    katex_fatal = ""
    node, node_path = find_node(args.node), find_node_path(args.node_path)
    if args.no_katex:
        katex_state = "跳过（--no-katex）"
    elif node and node_path:
        katex_errs, katex_fatal = verify_katex(results, node, node_path)
        katex_state = f"启用 katex {katex_version(node_path)}"
        if katex_fatal:
            katex_state = f"不可用（{katex_fatal}）"
            katex_errs = {}
    else:
        katex_state = "跳过（未探测到 node + katex，可用 --node/--node-path 指定）"

    failed = 0
    print(f"root : {root}")
    print(f"KaTeX: {katex_state}\n")
    print(f"{'文件':<44} {'公式':>8} {'图':>3} {'链':>4}  状态")
    print("-" * 78)
    for r in results:
        errs = [(t, e) for (rel, t), e in katex_errs.items() if rel == r["rel"]]
        bad = list(r["issues"])
        if errs:
            bad.append(f"{len(errs)} 个公式 KaTeX 报错")
        state = "OK" if not bad else "FAIL"
        if bad:
            failed += 1
        print(f"{r['rel']:<44} {len(r['math']):>4}/{r['n_disp']:<3} {r['imgs']:>3} {r['links']:>4}  {state}")
        for b in bad:
            print(f"    - {b}")
        for tex, e in errs[:5]:
            print(f"    - KaTeX: {e}\n      tex: {tex[:90]}")
        if len(errs) > 5:
            print(f"    - …还有 {len(errs) - 5} 个公式报错")

    print()
    if failed:
        print(f"{failed}/{len(results)} 个文件有问题。")
        return 1
    print(f"{len(results)} 个文件全部通过。")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
