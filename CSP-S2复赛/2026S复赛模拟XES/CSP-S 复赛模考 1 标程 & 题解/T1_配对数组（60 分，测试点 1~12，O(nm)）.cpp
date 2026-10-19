#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;

int n, m, a[N];

int main() {
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) cin >> a[i];

    // 枚举目标值 T，对每个配对用 O(1) 公式求最少操作次数：
    //   x+y == T          -> 0 次
    //   T in [1+min, m+max] -> 1 次（只改一个数即可）
    //   否则               -> 2 次
    // 时间复杂度 O(n*m)
    
    int ans = 1e18;
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
