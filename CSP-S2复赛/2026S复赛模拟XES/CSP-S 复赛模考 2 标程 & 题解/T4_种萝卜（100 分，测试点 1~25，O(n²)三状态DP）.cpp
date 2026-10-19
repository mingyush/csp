#include <bits/stdc++.h>
using namespace std;

using LL = long long;
const int N = 2005;

int n;
LL s[N][N];
LL f[N][N][2];

int main() {
    cin >> n;

    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j) {
            LL x;
            cin >> x;
            s[j][i] = s[j][i - 1] + x;
        }

    for (int p = 0; p <= n; ++p)
        f[1][p][0] = f[1][p][1] = 0;

    for (int j = 1; j < n; ++j) {
        LL pre = f[j][0][1] - s[j][0];
        for (int p = 1; p <= n; ++p) {
            LL v = max(f[j][p][0], pre + s[j][p]);
            f[j + 1][p][0] = f[j + 1][p][1] = v;
            pre = max(pre, f[j][p][1] - s[j][p]);
        }

        LL suf = f[j][n][0] + s[j + 1][n];
        for (int p = n - 1; p > 0; --p) {
            f[j + 1][p][0] = max(f[j + 1][p][0], suf - s[j + 1][p]);
            suf = max(suf, f[j][p][0] + s[j + 1][p]);
        }

        f[j + 1][0][0] = suf;
        f[j + 1][0][1] = max(f[j][0][0], f[j][n][0]);
    }

    LL ans = 0;
    for (int p = 0; p <= n; ++p)
        ans = max(ans, f[n][p][0]);

    cout << ans << '\n';
    return 0;
}
