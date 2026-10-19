#include <bits/stdc++.h>
using namespace std;

const int N = 2005;
const long long NEG = -(1LL << 60);

int n;
long long a[N][N], s[N][N];
long long f[N][N], g[N][N];
long long pre[N], suf[N];

long long solve_n3() {
    for (int j = 1; j <= n; ++j)
        for (int i = 1; i <= n; ++i)
            s[j][i] = s[j][i - 1] + a[i][j];

    for (int i = 0; i <= n; ++i)
        for (int j = 0; j <= n; ++j)
            f[i][j] = NEG;
    for (int i = 0; i <= n; ++i)
        f[i][0] = 0;

    for (int col = 1; col < n; ++col) {
        for (int i = 0; i <= n; ++i)
            for (int j = 0; j <= n; ++j)
                g[i][j] = NEG;

        for (int x = 0; x <= n; ++x) {
            pre[0] = f[x][0];
            for (int y = 1; y <= n; ++y)
                pre[y] = max(pre[y - 1], f[x][y]);

            suf[n + 1] = NEG;
            for (int y = n; y >= 0; --y) {
                long long v = f[x][y] == NEG ? NEG :
                    f[x][y] + max(0LL, s[col][y] - s[col][x]);
                suf[y] = max(suf[y + 1], v);
            }

            for (int y = 0; y <= n; ++y) {
                long long v1 = pre[y] == NEG ? NEG :
                    pre[y] + max(0LL, s[col][y] - s[col][x]);
                g[y][x] = max(v1, suf[y + 1]);
            }
        }

        for (int i = 0; i <= n; ++i)
            for (int j = 0; j <= n; ++j)
                f[i][j] = g[i][j];
    }

    long long ans = 0;
    for (int i = 0; i <= n; ++i)
        for (int j = 0; j <= n; ++j)
            ans = max(ans, f[i][j] + max(0LL, s[n][j] - s[n][i]));
    return ans;
}

long long solve_col() {
    long long w[N];
    for (int j = 1; j <= n; ++j)
        w[j] = a[1][j];

    long long dp[2][2], ndp[2][2];
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            dp[i][j] = NEG;
    dp[0][0] = dp[0][1] = 0;

    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
                ndp[j][k] = NEG;

        for (int x = 0; x < 2; ++x)
            for (int y = 0; y < 2; ++y)
                for (int z = 0; z < 2; ++z) {
                    if (dp[x][y] == NEG) continue;
                    long long add = y == 0 && (x || z) ? w[i] : 0;
                    ndp[y][z] = max(ndp[y][z], dp[x][y] + add);
                }

        memcpy(dp, ndp, sizeof(dp));
    }

    long long ans = 0;
    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 2; ++y) {
            if (dp[x][y] == NEG) continue;
            long long add = y == 0 && x ? w[n] : 0;
            ans = max(ans, dp[x][y] + add);
        }
    return ans * n;
}

int main() {
    cin >> n;
    long long c = -1;
    bool same = true, col = true;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j) {
            cin >> a[i][j];
            if (i == 1 && j == 1) c = a[i][j];
            else if (a[i][j] != c) same = false;
            if (i > 1 && a[i][j] != a[1][j]) col = false;
        }

    if (same)
        cout << c * n * (2LL * n / 3) << '\n';
    else if (col)
        cout << solve_col() << '\n';
    else
        cout << solve_n3() << '\n';
    return 0;
}
