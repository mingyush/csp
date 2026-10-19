#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;
const int M = 1e5 + 5;

int n, m, a[N], d[2 * M + 5];

int main() {
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) cin >> a[i];

    // d[T] 表示“把所有配对的和调整成 T”所需的总操作次数，
    // 用差分数组 + 前缀和进行区间加，最终扫描得到最小值。
    
    for (int i = 1; i <= n / 2; ++i) {
        int x = a[i], y = a[n - i + 1];
        int l = 1 + min(x, y), r = m + max(x, y);
        
        // 基础代价：每个配对默认要改 2 个数
        d[2] += 2; d[2 * m + 1] -= 2;
        // 和落在 [l, r] 内时，只需改 1 个数，代价减 1
        --d[l]; ++d[r + 1];
        // 和恰好等于 mid 时，无需改动，代价再减 1
        --d[x + y]; ++d[x + y + 1];
    }

    int ans = 1e9, res = 0;
    for (int T = 2; T <= 2 * m; ++T) {
        res += d[T];
        ans = min(ans, res);
    }
    
    cout << ans << '\n';
    return 0;
}
