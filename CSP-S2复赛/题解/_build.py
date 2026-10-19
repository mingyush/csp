#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""把 _head.md + _parts/各年份 + _tail.md 合并为单篇《CSP-S复赛真题题解（2021-2025）.md》"""
import os, re, sys

BASE = os.path.dirname(os.path.abspath(__file__))
PARTS = os.path.join(BASE, "_parts")
OUT = os.path.join(BASE, "CSP-S复赛真题题解（2021-2025）.md")
# 年份 -> 章节序号
YEARS = [("2025", "四"), ("2024", "五"), ("2023", "六"), ("2022", "七"), ("2021", "八")]

def load(name):
    txt = open(os.path.join(BASE, name), encoding="utf-8").read().strip()
    # 去掉文件末尾多余的分隔线，避免与拼接符叠加成两条 ---
    while txt.endswith("---"):
        txt = txt[:-3].rstrip()
    return txt

head = load("_head.md")
tail = load("_tail.md")

blocks = [head]
for y, num in YEARS:
    p = os.path.join(PARTS, f"{y}.md")
    if not os.path.exists(p):
        print(f"[skip] {p} missing", file=sys.stderr)
        continue
    txt = open(p, encoding="utf-8").read().rstrip()
    # 把 "## CSP-S YYYY 复赛题解" 提升为带章节号的 "## 四、CSP-S 2025 复赛题解"
    txt = re.sub(r"^##[ \t]*CSP-S[ \t]*" + y + r"[ \t]*复赛题解[ \t]*$",
                 f"## {num}、CSP-S {y} 复赛题解", txt, count=1, flags=re.M)
    blocks.append(txt)

blocks.append(tail)
doc = "\n\n---\n\n".join(b.strip() for b in blocks if b.strip()) + "\n"
open(OUT, "w", encoding="utf-8").write(doc)
print("wrote", OUT, len(doc), "chars,", doc.count("\n") + 1, "lines")
