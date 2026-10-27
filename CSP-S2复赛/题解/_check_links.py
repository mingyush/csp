#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""校验 题解/ 下所有 Markdown 的相对链接与页内锚点是否真实可达。

用法： python3 _check_links.py
"""
import os
import re
import sys
import unicodedata

ROOT = os.path.dirname(os.path.abspath(__file__))


def slug(text):
    """近似 GitHub 锚点算法"""
    s = text.strip().lower()
    s = re.sub(r"`", "", s)
    s = re.sub(r"[^\w\u4e00-\u9fff \-]", "", s)
    return s.replace(" ", "-")


def headings(path):
    txt = open(path, encoding="utf-8").read()
    out = []           # 保留重复项计数用
    seen = {}
    for m in re.finditer(r"^(#{1,6}) (.+?)\s*$", txt, re.M):
        base = slug(m.group(2))
        if base in seen:
            seen[base] += 1
            out.append(f"{base}-{seen[base]}")
        else:
            seen[base] = 0
            out.append(base)
    return out


LINK = re.compile(r"\[[^\]]*\]\(([^)\s]+)\)")
errors = []
checked = 0

for dirpath, dirnames, filenames in os.walk(ROOT):
    dirnames[:] = [d for d in dirnames if not d.startswith(".")]
    for fn in sorted(filenames):
        if not fn.endswith(".md"):
            continue
        path = os.path.join(dirpath, fn)
        rel = os.path.relpath(path, ROOT)
        txt = open(path, encoding="utf-8").read()
        # 去掉代码块，避免把代码里的括号当链接
        body = re.sub(r"```.*?```", "", txt, flags=re.S)
        for target in LINK.findall(body):
            if target.startswith(("http://", "https://", "mailto:")):
                continue
            checked += 1
            file_part, _, frag = target.partition("#")
            if not file_part:                      # 纯页内锚点
                if frag not in headings(path):
                    errors.append(f"{rel}: 锚点不存在 #{frag}")
                continue
            dest = os.path.normpath(os.path.join(dirpath, file_part))
            if not os.path.exists(dest):
                errors.append(f"{rel}: 目标不存在 {file_part}")
                continue
            if frag and dest.endswith(".md"):
                if frag not in headings(dest):
                    errors.append(f"{rel}: {file_part} 中锚点不存在 #{frag}")

print(f"检查 {checked} 条相对链接")
if errors:
    print(f"发现 {len(errors)} 处问题：")
    for e in errors:
        print("  -", e)
    sys.exit(1)
print("全部可达 ✔")
