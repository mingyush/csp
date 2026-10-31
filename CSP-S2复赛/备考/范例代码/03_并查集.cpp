#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

struct DSU {
    vector<int> fa, sz;
    DSU(int n) : fa(n + 1), sz(n + 1, 1) {
        iota(fa.begin(), fa.end(), 0);
    }
    int find(int x) {
        if (fa[x] == x) return x;
        return fa[x] = find(fa[x]);
    }
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b);
        fa[b] = a;
        sz[a] += sz[b];
    }
    bool same(int a, int b) {
        return find(a) == find(b);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    DSU dsu(n);
    while (q--) {
        char op;
        int a, b;
        cin >> op >> a >> b;
        if (op == 'M') dsu.unite(a, b);
        else if (op == 'Q') cout << (dsu.same(a, b) ? "Yes" : "No") << '\n';
    }
    return 0;
}
