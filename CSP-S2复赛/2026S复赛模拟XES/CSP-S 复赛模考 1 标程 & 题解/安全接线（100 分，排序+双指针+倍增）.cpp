#include <bits/stdc++.h>
using namespace std;

// 100 分标解：排序 + 双指针 + 倍增
//
// 关键洞察：边由「值差 <= d」定义，与原始编号无关。
// 将节点按 a 值排序后，每个节点的邻居是排序数组中一段连续区间，
// 且连通分量在排序数组中是连续的一段。
//
// 步骤：
//   1. 按 a 值排序，得到 pos，记 rk[原编号] = 排序后位置。
//   2. 双指针求 r[i] = 从位置 i 出发「一步最远跳」能到达的最右位置，
//      即满足 b[r[i]] - b[i] <= d 的最大下标。
//   3. 倍增 jump[k][i] = 从 i 连续做 2^k 次「最远跳」到达的位置。
//   4. 查询 (u,v)：映射到排序位置 x=rk[u], y=rk[v]（保证 x<=y），
//      用倍增贪心地从 x 跳到不超过 y 的最远位置，统计步数；
//      若最终仍到不了 y，则两点不在同一分量，输出 -1。
//
// 时间复杂度 O((n+q) log n)。

const int N = 2e5 + 5;
const int LOG = 20;

int n, q;
long long d, a[N];
int pos[N];       // 排序后第 i 位的原编号
int rk[N];        // 排序后，原编号 i 的位置
long long b[N];   // 排序后第 i 位的值
int r[N];         // 从位置 i 一步最远跳能到达的最右位置
int jump[LOG][N];

bool cmp(int x, int y) { return a[x] < a[y]; }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> q >> d;
    for (int i = 1; i <= n; ++i) cin >> a[i];

    // 1. 排序
    for (int i = 1; i <= n; ++i) pos[i] = i;
    sort(pos + 1, pos + n + 1, cmp);
    for (int i = 1; i <= n; ++i) {
        rk[pos[i]] = i;
        b[i] = a[pos[i]];
    }

    // 2. 双指针求一步最远跳 r[i]
    int j = 0;
    for (int i = 1; i <= n; ++i) {
        if (j < i) j = i;
        while (j + 1 <= n && b[j + 1] - b[i] <= d) ++j;
        r[i] = j;
    }

    // 3. 倍增
    for (int i = 1; i <= n; ++i) jump[0][i] = r[i];
    for (int k = 1; k < LOG; ++k)
        for (int i = 1; i <= n; ++i)
            jump[k][i] = jump[k - 1][jump[k - 1][i]];

    // 4. 查询
    while (q--) {
        int u, v;
        cin >> u >> v;
        int x = rk[u], y = rk[v];
        if (x > y) swap(x, y);
        if (x == y) { cout << "0\n"; continue; }

        // 贪心倍增：从 x 跳，尽量不越过 y
        int cur = x, ans = 0;
        for (int k = LOG - 1; k >= 0; --k)
            if (jump[k][cur] < y) {
                cur = jump[k][cur];
                ans += (1 << k);
            }
        // cur 现在是最后一个 < y 的位置，若再跳一步能到 >= y 则连通，否则 -1
        if (jump[0][cur] >= y)
            cout << ans + 1 << '\n';
        else
            cout << "-1\n";
    }
    return 0;
}
