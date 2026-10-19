#include <bits/stdc++.h>
using namespace std;

const int N = 505;
const long long NEG = -(1LL << 60);

int n;
long long a[N][N], s[N][N];
long long f[N][N], g[N][N];
long long pre[N], suf[N];

int main() {
    cin >> n;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            cin >> a[i][j];

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
    cout << ans << '\n';
    return 0;
}
