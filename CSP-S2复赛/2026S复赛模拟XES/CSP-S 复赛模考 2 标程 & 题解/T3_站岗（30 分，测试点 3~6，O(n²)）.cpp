#include <bits/stdc++.h>
using namespace std;

// 30 分：二分答案 + O(n²) 的 check（枚举起点 + 线性贪心）。
// 适用于 n <= 1000 的测试点（3~6）。
//
// 关键转化：正方形边界上的点按顺时针映射到一维周长坐标（周长 4*a），
// 曼哈顿距离在边界上就是沿边走的弧长，问题变成：
//   在环形数组上选 k 个数，任意相邻两个数（含首尾）距离 >= low。
//
// check(low)：枚举「第一个选中的点」作为起点（断环成链），往后线性贪心选点，
// 最后检查首尾环形距离。枚举起点 O(n) × 线性贪心 O(n) = O(n²)。

int n, k;
long long a;
vector<long long> pos;

bool check(long long low) {
    long long L = 4LL * a; // 周长
    for (int s = 0; s < n; ++s) {          // 枚举起点（断环）
        int cnt = 1;
        long long last = pos[s];
        for (int i = s + 1; i < n; ++i) {  // 往后线性贪心
            if (pos[i] - last >= low) {
                ++cnt;
                last = pos[i];
                if (cnt == k) break;
            }
        }
        // 首尾环形距离也要 >= low
        if (cnt == k && L - (last - pos[s]) >= low) return true;
    }
    return false;
}

int main() {
    cin >> n >> k >> a;
    pos.resize(n);
    for (int i = 0; i < n; ++i) {
        long long x, y;
        cin >> x >> y;
        long long p;
        if (x == 0) p = y;
        else if (y == a) p = a + x;
        else if (x == a) p = 3LL * a - y;
        else p = 4LL * a - x;
        pos[i] = p;
    }
    sort(pos.begin(), pos.end());

    long long lo = 1, hi = 4LL * a / k + 1;
    while (lo < hi) {
        long long mid = (lo + hi + 1) / 2;
        if (check(mid)) lo = mid;
        else hi = mid - 1;
    }
    cout << lo << '\n';
    return 0;
}
