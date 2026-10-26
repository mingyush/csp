#include <bits/stdc++.h>
using namespace std;

// 35 分：暴力模拟（测试点 1~7）。
//   - 测试点 1~3：n, q <= 1000，直接 O(nq) <= 1e6 模拟；
//   - 测试点 4~7：特殊性质 A（每个查询的 k > sqrt(n)），
//     一次查询最多影响 n/k < sqrt(n) 个位置，暴力总 O(q*sqrt(n)) 可过。
// 后续测试点用长区间、小步长构造，避免随机数据导致暴力实际修改次数过少。
// 注意：a[i] * v 可能超过 int 范围，必须用 1LL 转 long long 再取模，
//       否则测试点 3（专门卡没写 1LL 的写法）会出错。

const long long MOD = 1000000007LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    while (q--) {
        long long l, r, k, v;
        cin >> l >> r >> k >> v;
        --l; --r;
        for (long long idx = l; idx <= r; idx += k)
            a[idx] = a[idx] * v % MOD;
    }
    long long ans = 0;
    for (long long x : a) ans ^= x;
    cout << ans << '\n';
    return 0;
}
