#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

struct DSU {
    vector<int> fa;
    DSU(int n) : fa(n + 1) { iota(fa.begin(), fa.end(), 0); }
    int find(int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        fa[b] = a;
        return true;
    }
};

struct Edge {
    int u, v;
    long long w;
    bool operator<(const Edge& other) const { return w < other.w; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<Edge> e(m);
    for (auto &x : e) cin >> x.u >> x.v >> x.w;
    sort(e.begin(), e.end());

    DSU dsu(n);
    long long ans = 0;
    int used = 0;
    for (auto &x : e) {
        if (dsu.unite(x.u, x.v)) {
            ans += x.w;
            if (++used == n - 1) break;
        }
    }
    if (used == n - 1) cout << ans << '\n';
    else cout << "Impossible\n";
    return 0;
}
