# -*- coding: utf-8 -*-
"""一次性脚本：把模考1 各题面里已转写好的「测试点分布」表格回收进 images.json。

原图（datarange.jpeg）与 raw 快照都没进仓库，这里按旧脚本 _convert.py（已删除，见 git 历史）
里记录的图片 URL + 现行 md 里的表格正文补出清单，使「重新抓取 raw 后可复现同样的题面」。
运行一次即可，平时不需要。
"""
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2] / "模考1"

SRC = {
    "T1": "https://static.xiaohoucode.com/iucud_v1/335725/admin/image/c65e9a951dac432eabee30ac1c82c526.jpeg",
    "T2": "https://static.xiaohoucode.com/iucud_v1/335725/admin/image/d39677d2626045cfa5fd793a25ebc611.jpeg",
    "T3": "https://static.xiaohoucode.com/iucud_v1/335725/admin/image/11fece1bae8f425f8f79419b534899ae.jpeg",
    "T4": "https://static.xiaohoucode.com/iucud_v1/335725/admin/image/df1de49e352c4c13afc494cfe9454768.jpeg",
}


def extract_table(md: Path) -> tuple[str, str]:
    lines = md.read_text(encoding="utf-8").split("\n")
    start = next(i for i, ln in enumerate(lines) if ln.startswith("### 测试点分布"))
    end = start
    for i in range(start, len(lines)):
        if lines[i].startswith("|") or lines[i].startswith("###") or not lines[i].strip():
            end = i
        else:
            break
    while end > start and not lines[end].strip():
        end -= 1
    content = "\n".join(lines[start:end + 1]).strip()
    ctx = ""
    for j in range(start - 1, -1, -1):
        if lines[j].strip():
            ctx = re.sub(r"\s+", " ", lines[j].strip())[:80]
            break
    return content, ctx


def main() -> None:
    for tid, src in SRC.items():
        md = next((ROOT / tid).glob("*.md"))
        content, ctx = extract_table(md)
        title = md.read_text(encoding="utf-8").split("\n", 1)[0].lstrip("# ").strip()
        payload = {
            "_comment": "后期补齐的清单：raw 快照与 datarange.jpeg 未入库，"
                        "这里保存原图地址与已转写内容，重抓 raw 后可直接复现题面。"
                        "当前 md 已是最终态，无需再跑 inject。",
            "problem": title,
            "source_html": f"{md.stem}.html",
            "images": [{
                "id": "img1",
                "file": "datarange.jpeg",
                "src": src,
                "alt": "数据范围表",
                "section": "数据范围",
                "context": ctx,
                "content": content,
                "status": "transcribed",
            }],
        }
        (ROOT / tid / "images.json").write_text(
            json.dumps(payload, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        print(f"-> {tid}/images.json  content {len(content)}B  ctx={ctx[:40]}")


if __name__ == "__main__":
    main()
