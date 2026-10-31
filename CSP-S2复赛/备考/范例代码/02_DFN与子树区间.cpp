#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> g;
vector<int> dfn, sz;
int timer_cnt = 0;

void dfs(int u, int parent) {
    dfn[u] = ++timer_cnt; // 只在首次进入节点时编号
    sz[u] = 1;
    for (int v : g[u]) {
        if (v == parent) continue;
        dfs(v, u);
        sz[u] += sz[v];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    g.assign(n + 1, {});
    dfn.assign(n + 1, 0);
    sz.assign(n + 1, 0);

    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int u = 1; u <= n; ++u) sort(g[u].begin(), g[u].end());

    dfs(1, 0);
    for (int u = 1; u <= n; ++u) {
        cout << u << ": dfn=" << dfn[u]
             << ", subtree=[" << dfn[u] << ','
             << dfn[u] + sz[u] - 1 << "]\n";
    }
    return 0;
}
