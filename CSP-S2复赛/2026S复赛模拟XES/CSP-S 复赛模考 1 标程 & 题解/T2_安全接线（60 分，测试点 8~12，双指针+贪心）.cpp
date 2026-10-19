#include <bits/stdc++.h>
using namespace std;

// 60 分：排序 + 双指针 + 段内贪心跳。
// 排序后，每个节点的邻居是连续区间，连通分量是连续的一段。
// 先双指针求出每个位置「一步最远跳」r[i]；
// 查询 (u,v)：若不在同一连通块输出 -1，否则在段内从 x 贪心跳到 y 统计步数。
// 排序 O(n log n)，每次查询 O(n)（逐步贪心跳），总 O(n log n + q n)。
// 适用于 n <= 1e4, q <= 1e4（测试点 8~12）。

const int N = 1e4 + 5;

int n, q, d, a[N];
int pos[N];     // 排序后第 i 位的点编号
int rk[N];      // 排序后，点 i 的下标位置
int b[N];       // 排序后第 i 位的值
int r[N];       // 从位置 i 一步最远跳能到达的最右位置
int last[N];    // 位置 i 所属连通块的右边界

bool cmp(int x, int y) { return a[x] < a[y]; }

int main() {
    cin >> n >> q >> d;
    for (int i = 1; i <= n; ++i) cin >> a[i];

    // 排序
    for (int i = 1; i <= n; ++i) pos[i] = i;
    sort(pos + 1, pos + n + 1, cmp);
    for (int i = 1; i <= n; ++i) {
        rk[pos[i]] = i;
        b[i] = a[pos[i]];
    }

    // 双指针求一步最远跳 r[i]
    int j = 0;
    for (int i = 1; i <= n; ++i) {
        if (j < i) j = i;
        while (j + 1 <= n && b[j + 1] - b[i] <= d) ++j;
        r[i] = j;
    }

    // 求每个位置所属的连通块右边界（连续段）
    for (int i = 1; i <= n; ) {
        int p = i, cur = i;
        while (cur <= p) {
            p = max(p, r[cur]);
            ++cur;
        }
        for (int k = i; k <= p; ++k) last[k] = p;
        i = p + 1;
    }

    // 查询
    while (q--) {
        int u, v;
        cin >> u >> v;
        int x = rk[u], y = rk[v];
        if (x > y) swap(x, y);
        if (x == y) { cout << "0\n"; continue; }

        if (last[x] != last[y]) { cout << "-1\n"; continue; }

        int cur = x, ans = 0;
        while (cur < y) {
            cur = r[cur];
            ++ans;
        }
        cout << ans << '\n';
    }
    return 0;
}
