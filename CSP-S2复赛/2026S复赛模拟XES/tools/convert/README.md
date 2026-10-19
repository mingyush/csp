# tools/convert · 小猴编程题面转换工具

把在线 OJ（`static.xiaohoucode.com`）题面的 HTML 抓取快照，整理成**干净、公式可渲染、不依赖图片**的题面 Markdown。

三件事：**格式清洗**（HTML 结构 → 规范 Markdown）、**公式还原**（KaTeX 双份渲染 → 只留 LaTeX）、
**图片转内容**（图 → 文本，题面最终不引用任何图片）。

---

## 目录

```
tools/convert/
├── README.md                本文件
├── convert.py               HTML -> Markdown（parse / inject / images）
├── preview.py               Markdown -> 单页 HTML 预览（KaTeX 渲染）
├── check.py                 交付前自检（公式 / 围栏 / 链接）
├── _seed_mokao1.py          一次性脚本：补齐模考1 的 images.json（平时不用）
└── tests/
    ├── fixture_xiaohou.html 按真实 DOM 形状人工构造的夹具（raw 丢了，它是唯一护栏）
    └── test_convert.py      38 项回归测试，纯标准库
```

## 快速上手

在 `2026S复赛模拟XES/` 下执行。约定：raw 快照放 `<模考N>/_raw/`，文件名 `<题号><题名>.html`。

```bash
# 1. 转换：HTML -> 题面 md + 图片落盘 + images.json 待转写清单
python tools/convert/convert.py parse --raw 模考2/_raw

# 2. 看还有哪些图要转写（人工或模型逐张填 content）
python tools/convert/convert.py images --root 模考2 --format md

# 3. 填完 content 后回填进题面
python tools/convert/convert.py inject --root 模考2

# 4. 交付前自检 + 生成可读预览
python tools/convert/check.py   --root 模考2
python tools/convert/preview.py --root 模考2
```

`parse` 的 `--out` 默认取 raw 的上一级（`模考2/_raw` → `模考2`），也可显式指定。
`--only T1,T3` 只处理指定题；`--no-download` 只登记图片不下载。

## 解析规则（convert.py parse）

| 原页面 | 整理后 |
| --- | --- |
| `.top__item__name` | `# 标题` |
| `.top__item__tag`（时限 / 内存） | `> 时间限制：1s　内存限制：256M`（全角空格分隔） |
| `.sub-title` | `## 小节`，`【输入格式】` → `## 输入格式` |
| `.sample__list` | `## 输入输出样例` + `### 样例 #N` + `**输入#N**` / ```` ```text ```` |
| 无真实样例的 OI 测试点 | 归并成 `### 部分分测试点（无样例）` + 列表 |
| KaTeX 的 mathml / html 双份 | 只保留 `annotation[encoding="application/x-tex"]`，行内 `$…$`、独立成行 `$$…$$` |
| `<img>` | 落盘到题目目录并在 `images.json` 登记，题面里换成 `![alt](file)` |
| 数据范围表（图片） | 填 `images.json` 的 `content`，`inject` 后用 Markdown 表格替换图片 |

## 图片转内容（核心闭环）

题面最终**不允许依赖图片显示**，所以流程是「先登记、再转写、后回填」：

1. `parse` 下载图片到题目目录（默认与 md 同目录，`--img-dir` 可改成子目录），同时写 `images.json`；
2. 人工 / 多模态模型读图片，把内容写进 `content`（Markdown 片段，表格就用 Markdown 表格）；
3. `inject` 把 `![alt](file)` 整块替换成 `content`，并在文末追加一句原图出处注释。

`images.json` 字段：

| 字段 | 含义 |
| --- | --- |
| `id` / `file` | 图片编号与本地文件名（`img1.jpeg`；已有清单里的文件名会被沿用，不会改名） |
| `src` | 原始图片地址（合并清单时的主键） |
| `alt` / `section` / `context` | 图片说明、所在小节、就近一行正文，给转写者定位用 |
| `content` | **要填的东西**：图片内容的 Markdown 片段；留空表示还没转写 |
| `status` | `pending` / `transcribed` / `download-failed` / `content-without-ref` |

要点：

* `parse` 与 `inject` 都**幂等**：已有的 `file` / `content` / `status` 不会被覆盖，重复跑不会重复追加脚注。
* `content-without-ref` = 填了 `content` 但题面里找不到对应图片引用，通常是文件名被改过或 md 被手工编辑过。
* 图片只有"转写完成"和"还没转写"两种交付状态，**不要**把只带图片引用的题面交付出去——`images` 子命令就是用来把待办列干净的。
* 图片文件本身只是原始素材留档（体积大、可重新下载）；要不要入库按需决定。

## 小猴编程 DOM 坑位（改代码前先看这里）

这些是实测踩过的，`tests/fixture_xiaohou.html` 里每条都留了一份：

| # | 现象 | 处理 |
| --- | --- | --- |
| 1 | 一个 `.practice__desc` 可能把所有后续小节都吞进去（div 未闭合），不能按小节同级遍历 | 遍历全部 `div`，只对已知 class 反应；内容靠文档顺序而非层级 |
| 2 | 题目描述用裸 `<div>`（没有 `.desc__text`），`<ul>` 常是 `.desc__text` 的**兄弟**节点 | `.desc` 作为整体处理：跳过内层 `.desc__text`，保证块顺序正确、不重复 |
| 3 | KaTeX 公式有 mathml + html 两份渲染，直接取文本会得到乱码 | 只读 `annotation[encoding="application/x-tex"]` |
| 4 | 数据范围表是图片 | 走「图片转内容」闭环 |
| 5 | 时限块等整段正文重复出现 | 同一小节内对 ≥15 字的相同段落去重（`parse` 会打印去重条数） |
| 6 | 隐藏的 `desc-html` 副本导致正文重复 | 有 `desc-html` 祖先的节点整块丢弃 |
| 7 | 页面里的 HTML 注释（`Comment`）是 `NavigableString` 子类，会漏进正文 | 渲染前显式过滤 |
| 8 | `【样例解释 #N】` 出现在正文段落里 | 不做标题化，按原文加粗段落保留（只有 `.sub-title` 才升级为 `##`） |

**改解析规则的正确姿势**：先往 `tests/fixture_xiaohou.html` 里加一段复刻该现象的 HTML，
再改 `convert.py`，然后跑 `python tools/convert/tests/test_convert.py`。
raw 快照已经丢过一次，夹具是唯一还能拦住回归的东西。

## 交付前自检

```bash
python tools/convert/check.py --root 模考2
```

查四件事：代码围栏是否成对、`$` / `$$` 是否配对（残留裸 `$` 报行号）、
KaTeX 能否渲染、Markdown 链接与图片目标是否真实存在（路径含中文/空格会做 URL 解码）。

KaTeX 校验需要本地 `node` + `katex`；探测不到会自动降级跳过（不算失败），
可用 `--node` / `--node-path` 指定，或 `--no-katex` 显式关闭。

## 预览

```bash
python tools/convert/preview.py --root 模考2            # -> 模考2/题面预览.html
```

题目自动发现（跳过 `README.md` 与 `tools/`），按 T1…T12 自然序排列。
图片路径用 `os.path.relpath` 从 md 目录算到预览页所在目录——**这一点别改成手写前缀**，
老脚本就是靠 `"{tid}/"` 拼路径，预览页和题目目录一错位就 404。

公式是**先转义再交给 KaTeX**：auto-render 读的是 `textContent`，浏览器会把 `&lt;` 还原成 `<`，
所以 `$x < y$` 这类比较符必须转义，不能原样输出。

## 依赖

* Python 3.10+，`beautifulsoup4` + `lxml`
* `check.py` 的公式校验：`node` + `katex`（可选）

## 现状与待办

* **raw 快照没有入库**，模考1 的 `_raw/` 与 `datarange.jpeg` 都已丢失，因此模考1 只能手工维护。
  建议以后把 `_raw/*.html` 一起提交——它是唯一能让"重跑复现"成立的东西。
* 模考1 的 `images.json` 是事后按 git 历史里的图片地址 + 现行 md 的表格正文补出来的，
  只用于「重抓 raw 后可复现」；当前 md 已是最终态，`inject` 对它是 no-op。
* 已知原题问题：押题卷-02 T4 数据范围表里测试点 21~25 写 `$k \le 10^{12}$`，
  但题面约定 `$1 \le k \le \dfrac{n(n+1)}{2}$`，`$n \le 2 \times 10^5$` 时上界约 `$2 \times 10^{10}$`，
  两者矛盾（疑为原题笔误），题面保留原文。
* 解析规则针对的是当前版本的小猴编程题面页；站点改版后先更新 `tests/fixture_xiaohou.html` 再改代码。
