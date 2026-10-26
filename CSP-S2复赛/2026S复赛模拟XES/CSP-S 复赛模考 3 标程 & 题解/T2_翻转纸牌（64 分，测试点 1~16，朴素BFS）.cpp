#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;
int ans[N], vis[N];

// 64 分：朴素 BFS（测试点 1~16）。
// 位置统一使用 1-based。对当前所在位置 i，枚举所有包含 i 的合法区间左端点 l，
// 反转后王牌到达 j = 2l + k - 1 - i。

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p, k;
    cin >> n >> p >> k;
    int m;
    cin >> m;
    for (int i = 1; i <= m; ++i) {
        int b;
        cin >> b;
        vis[b] = 1;
    }

    for (int i = 1; i <= n; ++i) ans[i] = -1;
    queue<int> q;
    ans[p] = 0;
    vis[p] = 1;
    q.push(p);

    while (!q.empty()) {
        int i = q.front();
        q.pop();
        int L = max(1, i - k + 1);
        int R = min(i, n - k + 1);
        for (int l = L; l <= R; ++l) {
            int j = 2 * l + k - 1 - i;
            if (j < 1 || j > n) continue;
            if (vis[j]) continue;
            vis[j] = 1;
            ans[j] = ans[i] + 1;
            q.push(j);
        }
    }

    for (int i = 1; i <= n; ++i) cout << ans[i] << '\n';
    return 0;
}
