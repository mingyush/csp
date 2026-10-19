#include <bits/stdc++.h>
using namespace std;

// 75 分：特殊性质 A（d=0）与特殊性质 B（有序链）。
//
// 性质 A：d = 0，只有电压相同的两个接线点之间才能接一根线。
//   连通分量 = 相同电压的集合，组内是完全图。
//   答案 = (u==v ? 0 : (a[u]==a[v] ? 1 : -1))，O(1) 每次。
//
// 性质 B：电压非降序，且相邻差 <= d、隔一个差 > d，图是一条链。
//   答案 = (u==v ? 0 : |u-v|)，O(1) 每次。
//
// 这两个性质分别引导正解的「按值分组」和「排序后按位置差跳跃」两个核心想法。

int n, q;
long long d;
vector<long long> a;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> q >> d;
    a.resize(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    if (d == 0) {
        // 性质 A：同电压才连边
        while (q--) {
            int u, v;
            cin >> u >> v;
            if (u == v) cout << "0\n";
            else if (a[u] == a[v]) cout << "1\n";
            else cout << "-1\n";
        }
    } else {
        // 性质 B：有序链，距离 = 位置差
        while (q--) {
            int u, v;
            cin >> u >> v;
            if (u == v) cout << "0\n";
            else cout << abs(u - v) << '\n';
        }
    }
    return 0;
}
