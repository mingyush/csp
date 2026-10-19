#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""tools/convert 的回归测试（不依赖 pytest）。

用法：
    python tools/convert/tests/test_convert.py

覆盖：parse（HTML -> Markdown + 图片登记）-> preview（含图片相对路径）-> inject（content 回填）
     -> 幂等性 -> check.py 自检。raw 快照丢了以后，这套测试就是解析规则的唯一护栏。
"""
from __future__ import annotations

import json
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
TOOLS = HERE.parent
CONVERT = TOOLS / "convert.py"
PREVIEW = TOOLS / "preview.py"
CHECK = TOOLS / "check.py"
PY = sys.executable

TABLE = """### 测试点分布

| 测试点 | $n, m \\le$ | 特殊性质 |
| --- | --- | --- |
| 1 ~ 4 | 10 | 无 |
| 5 ~ 8 | $10^5$ | 无 |"""

results: list[tuple[bool, str, str]] = []


def ok(cond: bool, label: str, extra: str = "") -> None:
    results.append((bool(cond), label, extra))


def run(*args: str) -> str:
    proc = subprocess.run([PY, *args], capture_output=True, text=True)
    if proc.returncode != 0:
        raise AssertionError(f"命令失败：{PY} {' '.join(args)}\n{proc.stdout}\n{proc.stderr}")
    return proc.stdout


def main() -> int:
    tmp = Path(tempfile.mkdtemp(prefix="convert-test-"))
    raw = tmp / "_raw"
    raw.mkdir()
    shutil.copy(HERE / "fixture_xiaohou.html", raw / "T1配对系列.html")
    out = tmp / "out"

    # ---------------------------------------------------------------- parse
    log = run(str(CONVERT), "parse", "--raw", str(raw), "--out", str(out), "--no-download")
    md = out / "T1" / "T1配对系列.md"
    ok(md.exists(), "parse 生成 T1/T1配对系列.md", log.strip().splitlines()[-1] if log else "")
    text = md.read_text(encoding="utf-8")

    ok(text.startswith("# 配对序列\n"), "标题取自 .top__item__name")
    ok("> 时间限制：1s\u3000内存限制：256M" in text, "时限/内存标签拼成 blockquote")
    for head in ("## 题目描述", "## 输入格式", "## 输出格式", "## 输入输出样例", "## 数据范围"):
        ok(head in text, f"小节标题 {head}")
    ok("【输入格式】" not in text, "方括号标题被规范化")
    ok("### 样例 #1" in text, "样例编号规范化")
    ok("### 部分分测试点（无样例）" in text and "**样例 #2**：该组数据满足特殊性质。" in text,
       "OI 部分分测试点归并")
    ok("$$a_i + a_{n-i+1} = T$$" in text, "行间公式保留 $$ 且独立成行")
    ok("$n$" in text and "$n, m$" in text, "行内公式保留 $")
    ok("katex-mathml" not in text and "katex-html" not in text, "KaTeX 双份渲染已去重")
    ok("必须被丢弃" not in text, "desc-html 隐藏副本被丢弃")
    ok("坑 2" not in text and "坑 5" not in text, "HTML 注释不落进题面正文")
    ok(text.count("时间限制：1s\u3000内存限制：256M") == 2, "重复段落被去重（仅留标签行 + 正文 1 处）",
       f"实际 {text.count('时间限制：1s\u3000内存限制：256M')} 处")
    ok("- 性质 A：序列单调不减。" in text, "desc__text 的兄弟 <ul> 未丢失")
    ok("  - 子条目也要能渲染。" in text, "嵌套列表缩进渲染")
    ok("![数据范围表](img1.jpeg)" in text, "图片引用落到题目目录（同目录）")

    # ------------------------------------------------------------ images.json
    js_path = out / "T1" / "images.json"
    payload = json.loads(js_path.read_text(encoding="utf-8"))
    imgs = payload["images"]
    ok(len(imgs) == 1, "images.json 登记 1 张待转写图片", json.dumps(imgs, ensure_ascii=False)[:120])
    e = imgs[0]
    ok(e["status"] == "pending", "未转写时 status=pending")
    ok(e["section"] == "数据范围", "记录图片所在小节")
    ok(bool(e.get("context")), "记录图片上下文", e.get("context", ""))
    ok(e["file"] == "img1.jpeg", "按 URL 后缀命名图片文件")

    listing = run(str(CONVERT), "images", "--root", str(out))
    ok("img1.jpeg" in listing, "images 子命令列出待转写项")

    # --------------------------------------------------------------- preview
    prev = out / "题面预览.html"
    run(str(PREVIEW), "--root", str(out), "--out", str(prev))
    ok(prev.exists(), "preview 生成预览页")
    html = prev.read_text(encoding="utf-8")
    ok('src="T1/img1.jpeg"' in html, "预览页图片用 relpath 指到题目目录", )
    ok("<h2>配对序列" in html and "<h3>题目描述" in html, "标题层级映射（h1->h2）")
    ok("<!--" not in html, "HTML 注释不渲染进预览页")

    # ---------------------------------------------------------------- inject
    payload["images"][0]["content"] = TABLE
    js_path.write_text(json.dumps(payload, ensure_ascii=False, indent=2), encoding="utf-8")
    run(str(CONVERT), "inject", "--root", str(out))
    text2 = md.read_text(encoding="utf-8")
    ok("![数据范围表](img1.jpeg)" not in text2, "inject 用文本替换掉图片引用")
    ok("| 测试点 | $n, m \\le$ | 特殊性质 |" in text2, "转写内容（Markdown 表格）进入题面")
    ok("图片原图：img1.jpeg" in text2, "追加原图出处脚注")

    # ------------------------------------------------------------ 幂等 + 后续
    run(str(CONVERT), "inject", "--root", str(out))
    text3 = md.read_text(encoding="utf-8")
    ok(text3 == text2, "重复 inject 幂等")
    ok(text3.count("图片原图：img1.jpeg") == 1, "脚注不会重复追加")
    ok(json.loads(js_path.read_text(encoding="utf-8"))["images"][0]["status"] == "transcribed",
       "status 更新为 transcribed")

    run(str(PREVIEW), "--root", str(out), "--out", str(prev))
    ok('<table class="tbl">' in prev.read_text(encoding="utf-8"), "预览页渲染 Markdown 表格")

    chk = subprocess.run([PY, str(CHECK), "--root", str(out)], capture_output=True, text=True)
    ok(chk.returncode == 0, "check.py 自检通过", chk.stdout.strip().splitlines()[-1] if chk.stdout else "")

    # ---------------------------------------------------------------- 汇总
    bad = [r for r in results if not r[0]]
    for good, label, extra in results:
        print(f"  {'PASS' if good else 'FAIL'}  {label}" + (f"   [{extra}]" if extra and not good else ""))
    print(f"\n{len(results) - len(bad)}/{len(results)} 通过  ({tmp})")
    if bad:
        print("\n失败项：")
        for _g, label, extra in bad:
            print(f"  - {label}  {extra}")
    shutil.rmtree(tmp, ignore_errors=True)
    return 1 if bad else 0


if __name__ == "__main__":
    raise SystemExit(main())
