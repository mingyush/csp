#include <bits/stdc++.h>
using namespace std;

const int N = 105;
const long long NEG = -(1LL << 60);

int n;
long long a[N][N], s[N][N], f[N][N][N];

int main() {
    cin >> n;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            cin >> a[i][j];

    for (int j = 1; j <= n; ++j)
        for (int i = 1; i <= n; ++i)
            s[j][i] = s[j][i - 1] + a[i][j];

    for (int i = 1; i <= n; ++i)
        for (int j = 0; j <= n; ++j)
            for (int k = 0; k <= n; ++k)
                f[i][j][k] = NEG;

    for (int k = 0; k <= n; ++k)
        f[1][k][0] = 0;

    for (int i = 2; i <= n; ++i)
        for (int j = 0; j <= n; ++j)
            for (int k = 0; k <= n; ++k)
                for (int l = 0; l <= n; ++l)
                    f[i][j][k] = max(f[i][j][k],
                        f[i - 1][k][l] + max(0LL, s[i - 1][max(j, l)] - s[i - 1][k]));

    long long ans = 0;
    for (int j = 0; j <= n; ++j)
        for (int k = 0; k <= n; ++k)
            ans = max(ans, f[n][j][k] + max(0LL, s[n][k] - s[n][j]));
    cout << ans << '\n';
    return 0;
}
