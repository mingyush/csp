#include <bits/stdc++.h>
using namespace std;

// 55 分：暴力 + k = 1 差分（测试点 1~11）。
//   - 测试点 1~7：暴力模拟（k 大时一次查询影响 < sqrt(n) 个位置，O(q*sqrt(n)) 可过）；
//   - 测试点 8~11：特殊性质 B（k = 1），是普通区间乘法，用「乘法差分 + 前缀积」O(n+q)。
// k = 1 的差分：在 l 处乘 v，在 r+1 处乘 v 的逆元（费马小定理），前缀累乘得到每位置的乘数。

const long long MOD = 1000000007LL;

long long qpow(long long a, long long b) {
    long long r = 1;
    a %= MOD;
    while (b) {
        if (b & 1) r = r * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // 读入所有操作，k==1 的做差分，k>1 的直接暴力
    vector<long long> diff(n + 1, 1);
    while (q--) {
        long long l, r, k, v;
        cin >> l >> r >> k >> v;
        --l; --r;
        if (k == 1) {
            diff[l] = diff[l] * v % MOD;
            diff[r + 1] = diff[r + 1] * qpow(v, MOD - 2) % MOD;
        } else {
            for (long long idx = l; idx <= r; idx += k)
                a[idx] = a[idx] * v % MOD;
        }
    }
    long long mul = 1;
    for (int i = 0; i < n; ++i) {
        mul = mul * diff[i] % MOD;
        a[i] = a[i] * mul % MOD;
    }
    long long ans = 0;
    for (long long x : a) ans ^= x;
    cout << ans << '\n';
    return 0;
}
