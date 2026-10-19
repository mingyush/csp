#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;

int n, m, a[N];

int main() {
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) cin >> a[i];

    // 特殊性质：所有 a_i 都相等，设为 v。
    // 每个配对都是 (v, v)，和为 2v。令 T = 2v 即可让所有配对都无需改动，
    // 故答案恒为 0。时间复杂度 O(n)。
    // 覆盖测试点 13~14，与 O(nm) 组合后累计 70 分。
    bool all = true;
    for (int i = 2; i <= n; ++i)
        if (a[i] != a[1]) {
            all = false;
            break;
        }
    if (all) {
        cout << 0 << '\n';
        return 0;
    }

    // 不满足特殊性质时，退化为 O(nm)（覆盖测试点 1~12）。
    int ans = 1e9;
    for (int T = 2; T <= 2 * m; ++T) {
        int res = 0;
        for (int i = 1; i <= n / 2; ++i) {
            int x = a[i], y = a[n - i + 1];
            if (x + y == T) continue;
            
            int l = 1 + min(x, y), r = m + max(x, y);
            if (l <= T && T <= r) ++res;
            else res += 2;
        }
        ans = min(ans, res);
    }
    
    cout << ans << '\n';
    return 0;
}
