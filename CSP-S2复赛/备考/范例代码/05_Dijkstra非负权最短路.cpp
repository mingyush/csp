#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
using ll = long long;
const ll INF = (1LL << 62);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s;
    cin >> n >> m >> s;
    vector<vector<pair<int,int>>> g(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w}); // 本例是无向图
    }

    vector<ll> dist(n + 1, INF);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    dist[s] = 0;
    pq.push({0, s});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }

    for (int v = 1; v <= n; ++v) {
        if (dist[v] == INF) cout << "INF";
        else cout << dist[v];
        cout << (v == n ? '\n' : ' ');
    }
    return 0;
}
