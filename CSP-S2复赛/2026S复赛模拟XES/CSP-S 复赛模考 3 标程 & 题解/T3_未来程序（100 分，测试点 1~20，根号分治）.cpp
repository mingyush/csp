#include <bits/stdc++.h>
using namespace std;

// 100 分：根号分治（测试点 1~20）。
//
// 每个查询 [l, r, k, v] 把下标 l, l+k, l+2k, ... (<= r) 的元素都乘 v（模 1e9+7）。
// 设 B = floor(sqrt(n)) + 1：
//   大 k（k > B）：一次查询最多影响 n/k < B 个位置，直接暴力，总 O(q*B)。
//   小 k（k <= B）：按 (k, l%k) 分组，每组内位置构成等差数列，用「乘法差分 + 前缀积」批量处理。
//       差分乘 v，在区间右端 +1 处乘 v 的逆元（费马小定理求逆），
//       前缀累乘即得到每个位置的乘数。总 O(n*B)。
// 总复杂度 O((n+q)*sqrt(n))。
//
// 特殊性质 C（测试点 12~16，1 <= v <= 5）时，逆元可打表（5 个常数），
// 但本正解统一用费马小定理求逆，对任意 v 都成立，也覆盖无性质的 17~20。

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

    int B = (int)sqrt(n) + 1;

    vector<array<long long, 4>> big; // l, r, k, v（k > B）
    vector<vector<array<long long, 3>>> small(B + 1); // small[k] = {l, r, v}

    for (int i = 0; i < q; ++i) {
        long long l, r, k, v;
        cin >> l >> r >> k >> v;
        --l; --r; // 转 0-indexed
        if (k > B) big.push_back({l, r, k, v});
        else small[(int)k].push_back({l, r, v});
    }

    // 大 k 直接暴力
    for (auto &qq : big) {
        long long l = qq[0], r = qq[1], k = qq[2], v = qq[3];
        for (long long idx = l; idx <= r; idx += k)
            a[idx] = a[idx] * v % MOD;
    }

    // 小 k 按余数分组差分
    for (int k = 1; k <= B; ++k) {
        if (small[k].empty()) continue;
        vector<vector<array<long long, 3>>> bucket(k);
        for (auto &qq : small[k]) {
            bucket[qq[0] % k].push_back(qq);
        }
        for (int r0 = 0; r0 < k; ++r0) {
            if (bucket[r0].empty()) continue;
            int max_t = (n - 1 - r0) / k;
            if (max_t < 0) continue;
            vector<long long> diff(max_t + 2, 1);
            for (auto &qq : bucket[r0]) {
                long long l = qq[0], r = qq[1], v = qq[2];
                int t1 = (int)((l - r0) / k);
                int t2 = (int)((r - r0) / k);
                diff[t1] = diff[t1] * v % MOD;
                if (t2 + 1 <= max_t) {
                    long long inv = qpow(v, MOD - 2);
                    diff[t2 + 1] = diff[t2 + 1] * inv % MOD;
                }
            }
            long long mul = 1;
            for (int t = 0; t <= max_t; ++t) {
                mul = mul * diff[t] % MOD;
                if (mul != 1) {
                    int idx = r0 + t * k;
                    a[idx] = a[idx] * mul % MOD;
                }
            }
        }
    }

    long long ans = 0;
    for (long long x : a) ans ^= x;
    cout << ans << '\n';
    return 0;
}
