#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""把 _head.md + _parts/各年份 + _tail.md 组装成两种形态的题解文档：

  1) 合订本   CSP-S复赛真题题解（2021-2025）.md      —— 单篇全量，供整体阅读/打印
  2) 分册     分册/00-总览与索引.md                  —— 说明 + 总览表 + 分类索引 + 分册入口
              分册/2025.md … 分册/2021.md            —— 每年一份，含本场速览与页内跳转

分片是唯一事实来源；改内容只改 _parts/*.md 或 _head.md / _tail.md，再跑本脚本。

用法： python3 _build.py
"""
import os
import re

BASE = os.path.dirname(os.path.abspath(__file__))
PARTS = os.path.join(BASE, "_parts")
SPLIT = os.path.join(BASE, "分册")
BOOK_NAME = "CSP-S复赛真题题解（2021-2025）.md"
BOOK = os.path.join(BASE, BOOK_NAME)

# (年份, 合订本章节号, 分册序号)
YEARS = [
    ("2025", "四", 1),
    ("2024", "五", 2),
    ("2023", "六", 3),
    ("2022", "七", 4),
    ("2021", "八", 5),
]
TOTAL = len(YEARS)


def read(name):
    return open(os.path.join(BASE, name), encoding="utf-8").read()


def strip_rule(txt):
    """去掉文末的分隔线，避免与拼接符叠加成两条 ---"""
    txt = txt.strip()
    while txt.endswith("---"):
        txt = txt[:-3].rstrip()
    return txt


def anchor(heading):
    """按 GitHub 规则粗算锚点：小写、去标点、空格转连字符"""
    s = heading.strip().lower()
    s = re.sub(r"[^\w\u4e00-\u9fff \-]", "", s)
    return "#" + s.replace(" ", "-")


def overview_rows(head):
    """从 _head.md 总览表解析 {年份: [(T号, 题名, 英文名, 洛谷号, 核心算法, 难度), ...]}"""
    rows = {}
    pat = re.compile(
        r"^\| (20\d\d) \| (T\d) \| ([^|]+) \| `([^`]+)` \| (P\d+) \| ([^|]+) \| ([^|]+) \|$",
        re.M,
    )
    for m in pat.finditer(head):
        y, t, name, en, luo, alg, dif = (x.strip() for x in m.groups())
        rows.setdefault(y, []).append((t, name, en, luo, alg, dif))
    return rows


def problem_list(part_text):
    """分片里的 ### T1 社团招新（club） -> [('T1', '社团招新（club）')]"""
    return [(m.group(1), m.group(2).strip())
            for m in re.finditer(r"^### (T\d) (.+)$", part_text, re.M)]


# ---------------------------------------------------------------- 合订本

def build_book(head, parts, tail):
    blocks = [strip_rule(head)]
    for y, num, _ in YEARS:
        txt = parts[y].rstrip()
        # "## CSP-S YYYY 复赛题解" 提升为 "## 四、CSP-S YYYY 复赛题解"
        txt, n = re.subn(
            r"^##[ \t]*CSP-S[ \t]*" + y + r"[ \t]*复赛题解[ \t]*$",
            f"## {num}、CSP-S {y} 复赛题解",
            txt, count=1, flags=re.M,
        )
        assert n == 1, f"[{y}] 合订本章节标题未命中"
        blocks.append(strip_rule(txt))
    blocks.append(strip_rule(tail))
    doc = "\n\n---\n\n".join(b for b in blocks if b) + "\n"
    open(BOOK, "w", encoding="utf-8").write(doc)
    print(f"[book] {BOOK_NAME}  {doc.count(chr(10)) + 1} 行")


# ---------------------------------------------------------------- 分册：单年

def build_year(y, idx, part_text, rows):
    probs = problem_list(part_text)
    assert len(probs) == 4, f"[{y}] 题目数 {len(probs)} != 4"

    body = re.sub(r"^##[ \t]*CSP-S[ \t]*" + y + r"[ \t]*复赛题解[ \t]*\n", "",
                  part_text.rstrip(), count=1)
    body = body.lstrip("\n")
    # ### T1 -> ## T1，题目在分册里成为二级标题；顺手去掉文末分隔线，避免与页脚分隔线重复
    body = strip_rule(re.sub(r"^### (T\d) ", r"## \1 ", body, flags=re.M))

    nav = ["[总览与索引](00-总览与索引.md)", f"[合订本全文](../{BOOK_NAME})"]
    adj = []
    if idx > 1:
        adj.append(f"[← {YEARS[idx - 2][0]}]({YEARS[idx - 2][0]}.md)")
    if idx < TOTAL:
        adj.append(f"[{YEARS[idx][0]} →]({YEARS[idx][0]}.md)")
    nav += adj

    jumps = " ｜ ".join(f"[{t} {rest}]({anchor(t + ' ' + rest)})" for t, rest in probs)
    footer = "↑ 返回 [总览与索引](00-总览与索引.md)"
    if adj:
        footer += " ｜ 相邻分册：" + " ｜ ".join(adj)

    out = [
        f"# CSP-S {y} 复赛题解",
        "",
        f"> 《CSP-S 复赛真题题解（2021—2025）》分册 {idx}/{TOTAL}，覆盖本场 4 题。",
        "> 导航：" + " ｜ ".join(nav),
        "",
        "## 本场速览",
        "",
        "| 题号 | 题目 | 英文目录名 | 洛谷题号 | 核心算法 | 难度 |",
        "| --- | --- | --- | --- | --- | --- |",
    ]
    for (t, name, en, luo, alg, dif) in rows[y]:
        out.append(f"| {t} | {name} | `{en}` | {luo} | {alg} | {dif} |")

    out += ["", f"**页内跳转**：{jumps}", "", "---", "",
            body, "", "---", "", footer, ""]

    doc = "\n".join(out)
    open(os.path.join(SPLIT, f"{y}.md"), "w", encoding="utf-8").write(doc)
    print(f"[year] 分册/{y}.md  {doc.count(chr(10)) + 1} 行")


# ---------------------------------------------------------------- 分册：总览与索引

def build_index(head, tail, rows):
    m = re.match(r"# [^\n]*\n\n((?:> [^\n]*\n)+)", head)
    assert m, "_head.md 开头的标题/引言未匹配"
    intro = m.group(1).strip()
    rest = re.sub(r"^---\n+", "", head[m.end():].lstrip("\n"))

    # 分册比合订本多一层目录，_head.md 里的相对链接要跟着上跳一级
    rest = rest.replace("](../真题/", "](../../真题/").replace("](../备考/", "](../../备考/")
    # 合订本里指向「第八章」的交叉引用，在分册里换成文件名链接
    rest = rest.replace("（原因见第八章 T4 末尾）",
                        "（原因见 [2021 分册](2021.md) 的 T4 末尾）")

    entry = [
        "## 分册入口",
        "",
        f"全部内容按年份拆成 {TOTAL} 份，外加本篇总览。手机/单屏阅读建议直接进年份分册；"
        f"要一次通读或打印用[合订本](../{BOOK_NAME})。",
        "",
        "| 分册 | 本场 4 题 | 洛谷题号 |",
        "| --- | --- | --- |",
    ]
    for y, _, idx in YEARS:
        names = " · ".join(r[1] for r in rows[y])
        entry.append(f"| [{y}]({y}.md)（第 {idx} 册） | {names} | {rows[y][0][3]}—{rows[y][-1][3]} |")

    t = strip_rule(tail)
    t, n = re.subn(r"^## 九、待办与后续", "## 四、待办与后续", t, count=1, flags=re.M)
    assert n == 1, "_tail.md 章节号未命中"

    doc = "\n\n".join([
        "# CSP-S 复赛真题题解 · 总览与索引",
        intro,
        "> 本册是系列的总览入口：说明与来源、五年 20 题总览、验证状态、按算法分类索引、分册入口；逐题内容在各年份分册里。",
        "---",
        "\n".join(entry),
        "---",
        rest,
        "---",
        t,
        "",
    ]).strip() + "\n"

    open(os.path.join(SPLIT, "00-总览与索引.md"), "w", encoding="utf-8").write(doc)
    print(f"[index] 分册/00-总览与索引.md  {doc.count(chr(10)) + 1} 行")


def main():
    os.makedirs(SPLIT, exist_ok=True)
    head = read("_head.md")
    tail = read("_tail.md")
    parts = {y: read(os.path.join("_parts", f"{y}.md")) for y, _, _ in YEARS}
    rows = overview_rows(head)
    missing = [y for y, _, _ in YEARS if y not in rows]
    assert not missing, f"总览表缺年份：{missing}"

    build_book(head, parts, tail)
    for y, _, idx in YEARS:
        build_year(y, idx, parts[y], rows)
    build_index(head, tail, rows)


if __name__ == "__main__":
    main()
