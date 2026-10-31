# 对拍最小范例

目标：同一份随机小数据，让"暴力正确程序"和"待验证程序"各算一次；输出不同就立即停下，保留反例。

对拍不能证明程序正确，只能证明"这组数据上还没错"。它的作用是在交卷前把明显的错误逼出来，并把反例留成可复查的文件。

## 文件

| 文件 | 作用 |
| --- | --- |
| `gen.cpp` | 随机生成小数据 |
| `brute.cpp` | O(n²) 暴力，作为正确答案 |
| `solution.cpp` | O(n) Kadane，作为待验证程序 |
| `stress_windows.cpp` | Windows 驱动，用 `fc.exe` 比较 |
| `stress_linux.cpp` | Linux／NOI Linux 驱动，用 `diff -q` 比较 |

## Windows

CMD：

```bat
g++ gen.cpp -O2 -std=c++14 -o gen.exe
g++ brute.cpp -O2 -std=c++14 -o brute.exe
g++ solution.cpp -O2 -std=c++14 -o solution.exe
g++ stress_windows.cpp -O2 -std=c++14 -o stress_windows.exe
stress_windows.exe
```

PowerShell 最后一行写 `./stress_windows.exe` 或 `.\stress_windows.exe`。手工比较写 `fc.exe brute.txt solution.txt`，不要只写 `fc`，避免命中 PowerShell 的同名别名。

## Linux / NOI Linux

```bash
g++ gen.cpp -O2 -std=c++14 -o gen
g++ brute.cpp -O2 -std=c++14 -o brute
g++ solution.cpp -O2 -std=c++14 -o solution
g++ stress_linux.cpp -O2 -std=c++14 -o stress_linux
./stress_linux
```

手工比较：`diff -q brute.txt solution.txt`。

## 一个能立刻上手的练法

1. 先原样运行，确认连续通过——不对拍成功一次，就分不清后面停下是"发现了 bug"还是"脚本本身坏了"。
2. 把 `solution.cpp` 里的 `long long ans = a[1];` 故意改成 `long long ans = 0;`。
3. 重新编译 solution，再跑对拍。
4. 应当很快停下，并留下 `input.txt / brute.txt / solution.txt`。
5. 看懂为什么"全负数"会让错误程序输出 0——这就是最小反例的形态。

这里没有用 `freopen`：输入输出文件由 shell 的 `<` 和 `>` 重定向完成，避免在练习阶段就被文件名与路径问题干扰。考场是否需要重定向，按当场题面与说明执行，见 [防爆零与提交检查清单](../防爆零与提交检查清单.md)。

## 用对拍时的三个约束

- **brute 必须先可信**：暴力本身错了，对拍只会把两个错误对齐。先用样例和手算验证 brute。
- **随机范围要包含边界**：全相等、全负、单点、极值、规模刚好跨越阈值，这些才是错误高发区；纯随机大范围数据经常测不出容量与初始化问题。
- **停下就保存**：反例文件是复盘证据，比"我记得刚才错了一次"有用得多；反例的三种输出（输入、正确、实际）要成组保留。
