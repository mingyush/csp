#include <bits/stdc++.h>
using namespace std;

// 10 分：暴力枚举所有 C(n,k) 种选法，取最小曼哈顿距离的最大值。
// 适用于 n <= 15 的测试点（1~2）。

int n, k;
long long a;
vector<long long> x, y;

int main() {
    cin >> n >> k >> a;
    x.resize(n); y.resize(n);
    for (int i = 0; i < n; ++i) cin >> x[i] >> y[i];

    long long ans = 0;
    // 枚举所有 k 个下标的组合
    vector<int> idx(k);
    for (int i = 0; i < k; ++i) idx[i] = i;
    while (true) {
        long long mn = LLONG_MAX;
        for (int p = 0; p < k; ++p)
            for (int q = p + 1; q < k; ++q) {
                long long d = llabs(x[idx[p]] - x[idx[q]]) + llabs(y[idx[p]] - y[idx[q]]);
                mn = min(mn, d);
            }
        ans = max(ans, mn);
        // 下一个组合
        int j = k - 1;
        while (j >= 0 && idx[j] == n - k + j) --j;
        if (j < 0) break;
        ++idx[j];
        for (int t = j + 1; t < k; ++t) idx[t] = idx[t - 1] + 1;
    }
    cout << ans << '\n';
    return 0;
}
