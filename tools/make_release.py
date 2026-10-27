#!/usr/bin/env python3
"""打包 Release 附件。

用法：
    python3 tools/make_release.py            # 跑全部，产物写入 dist/
    python3 tools/make_release.py solutions  # 只打某一个包

产物：
    dist/csp-s1-papers.zip      J1 + S1 初赛真题（PDF/MD，按年份）
    dist/csp-s2-solutions.zip   S 组复赛 2021—2025 逐题题解（含分册）
    dist/csp-s2-papers.zip      S 组复赛真题与模拟考
    dist/csp-full.zip           全仓库快照（仅 git 跟踪的文件）

为什么要打这些包：仓库约 178MB，很多同学卡在 git clone 上。
Release 附件让人不装 Git 也能直接拿到要用的那一摞 PDF。
"""
from __future__ import annotations

import argparse
import subprocess
import sys
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DIST = ROOT / "dist"

# name -> 要打包的目录/文件（相对仓库根）
PACKAGES: dict[str, list[str]] = {
    "s1-papers": [
        "CSP-J1初赛/真题",
        "CSP-S1初赛/真题",
    ],
    "s2-solutions": [
        "CSP-S2复赛/题解",
    ],
    "s2-papers": [
        "CSP-S2复赛/真题",
        "CSP-S2复赛/2026S复赛模拟XES",
        "CSP-S2复赛/2026S复赛冲刺班YU",
    ],
}


def add_path(zf: zipfile.ZipFile, rel: str) -> int:
    """把一个文件或目录写进 zip，返回写入的文件数。"""
    base = ROOT / rel
    if not base.exists():
        print(f"  ! 跳过不存在的路径: {rel}")
        return 0
    if base.is_file():
        zf.write(base, rel)
        return 1
    n = 0
    for f in sorted(base.rglob("*")):
        if f.is_file() and f.name != ".DS_Store":
            zf.write(f, f.relative_to(ROOT).as_posix())
            n += 1
    return n


def build(name: str) -> Path:
    paths = PACKAGES[name]
    out = DIST / f"csp-{name}.zip"
    with zipfile.ZipFile(out, "w", zipfile.ZIP_DEFLATED, compresslevel=9) as zf:
        total = sum(add_path(zf, p) for p in paths)
    size = out.stat().st_size / 1024 / 1024
    print(f"✓ {out.relative_to(ROOT)}  {total} 个文件  {size:.1f}MB")
    return out


def build_full() -> Path:
    """全仓库快照：用 git archive，只含被跟踪的文件，自动排除 .git 与临时文件。"""
    out = DIST / "csp-full.zip"
    with open(out, "wb") as fh:
        r = subprocess.run(
            ["git", "archive", "--format=zip", "-9", "HEAD"],
            cwd=ROOT, stdout=fh, stderr=subprocess.PIPE,
        )
    if r.returncode != 0:
        raise SystemExit(f"git archive 失败: {r.stderr.decode()}")
    size = out.stat().st_size / 1024 / 1024
    print(f"✓ {out.relative_to(ROOT)}  全仓库快照  {size:.1f}MB")
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description="打包 Release 附件")
    ap.add_argument("only", nargs="?", help="只打某一个包：s1-papers / s2-solutions / s2-papers / full")
    args = ap.parse_args()

    if subprocess.run(["git", "rev-parse", "--git-dir"], cwd=ROOT,
                      capture_output=True).returncode != 0:
        print("错误：这里不是 git 仓库")
        return 1

    DIST.mkdir(exist_ok=True)
    targets = [args.only] if args.only else list(PACKAGES) + ["full"]
    for t in targets:
        if t == "full":
            build_full()
        elif t in PACKAGES:
            build(t)
        else:
            print(f"未知包名: {t}\n可选: {', '.join(list(PACKAGES) + ['full'])}")
            return 1

    print("\n下一步（需要 gh 或网页上传）：")
    tag = subprocess.run(["git", "describe", "--tags", "--always"], cwd=ROOT,
                         capture_output=True, text=True).stdout.strip()
    print(f"  gh release create <tag> dist/*.zip -t \"<标题>\" -n \"<说明>\"")
    print(f"  当前 HEAD: {tag}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
