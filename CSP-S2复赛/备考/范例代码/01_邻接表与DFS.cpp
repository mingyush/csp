#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> g;
vector<int> vis;

void dfs(int u) {
    vis[u] = 1;
    cout << u << ' ';
    for (int v : g[u]) {
        if (!vis[v]) dfs(v);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    g.assign(n + 1, {});
    vis.assign(n + 1, 0);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u); // 无向图要加两次
    }
    for (int u = 1; u <= n; ++u) sort(g[u].begin(), g[u].end());

    dfs(1);
    cout << '\n';
    return 0;
}
