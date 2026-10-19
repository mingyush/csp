#include <bits/stdc++.h>
using namespace std;

// 35 分：Floyd 全源最短路 O(n^3)，每次询问 O(1)。
// 建图 O(n^2) + Floyd O(n^3)。
// 适用于 n <= 200（测试点 3~7）。

const int N = 205;

int n, q, d, a[N], f[N][N];

int main() {
    cin >> n >> q >> d;
    for (int i = 1; i <= n; ++i) cin >> a[i];

    memset(f, 0x3f, sizeof f);
    for (int i = 1; i <= n; ++i) f[i][i] = 0;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            if (i != j && abs(a[i] - a[j]) <= d) 
                f[i][j] = 1;

    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j)
                f[i][j] = min(f[i][j], f[i][k] + f[k][j]);

    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << (f[u][v] >= 1e9 ? -1 : f[u][v]) << '\n';
    }
    return 0;
}
