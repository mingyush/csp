#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;

int n, m, a[N];

// 对固定配对 (x, y) 和目标值 T，用最朴素的方式求最少操作次数：
// 枚举“改哪一个数”以及“改成 [1,m] 中的哪个值”，判断能否让 x+y == T。
// 单个配对 O(m)，总复杂度 O(n*m^2)。

int Calc(int x, int y, int T) {
    if (x + y == T) return 0;
    int res = 2; // 最坏改两个数
    
    // 只改 x
    for (int nx = 1; nx <= m; ++nx)
        if (nx + y == T)
            res = 1;
        
    // 只改 y
    for (int ny = 1; ny <= m; ++ny)
        if (x + ny == T)
            res = 1;
            
    return res;
}

int main() {
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) cin >> a[i];

    // 暴力：枚举目标值 T，对每个配对枚举改法。
    // 时间复杂度 O(n*m^2)，适用于 n, m <= 100（测试点 1~8，累计 40 分）。
    int ans = 1e9;
    for (int T = 2; T <= 2 * m; ++T) {
        int res = 0;
        for (int i = 1; i <= n / 2; ++i)
            res += Calc(a[i], a[n - i + 1], T);
        ans = min(ans, res);
    }
    
    cout << ans << '\n';
    return 0;
}
