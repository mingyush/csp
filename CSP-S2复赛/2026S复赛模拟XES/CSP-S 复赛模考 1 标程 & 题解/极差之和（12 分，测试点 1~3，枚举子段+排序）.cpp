#include <bits/stdc++.h>
using namespace std;

const int N = 10005;

int n;
long long k, a[N];
long long b[N];

// 枚举子段，排序取前 k 大。
int main() {
    cin >> n >> k;
    for (int i = 1; i <= n; ++i) cin >> a[i];

    int tot = 0;
    for (int l = 1; l <= n; ++l) {
        long long mn = a[l], mx = a[l];
        for (int r = l; r <= n; ++r) {
            mn = min(mn, a[r]);
            mx = max(mx, a[r]);
            b[++tot] = mx - mn;
        }
    }
    sort(b + 1, b + tot + 1, greater<long long>());
    long long ans = 0;
    for (int i = 1; i <= k; ++i) ans += b[i];
    cout << ans << '\n';
    return 0;
}
