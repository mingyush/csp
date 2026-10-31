// CSP-S 2026: small, reusable C++14 templates for last-minute practice.
// Study only: do not bring this file or any storage device into the exam.
// These are functions, NOT solutions to a particular problem.
// All graph vertices below are numbered 1..n, unless stated otherwise.
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 1. DSU / union-find: connectivity, connected components, Kruskal.
// Usage: DSU dsu(n); if(dsu.unite(u,v)) { /* newly connected */ }
struct DSU {
    vector<int> parent, sz;
    DSU(int n) : parent(n + 1), sz(n + 1, 1) {
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

// 2. Dijkstra: single-source shortest path, ONLY nonnegative edge weights.
// g[u] contains (neighbor, edgeWeight). Unreachable => INF.
vector<ll> dijkstra(int n, int source,
                    const vector<vector<pair<int, ll>>>& g) {
    const ll INF = (1LL << 62);
    vector<ll> dist(n + 1, INF);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    dist[source] = 0;
    pq.push({0, source});
    while (!pq.empty()) {
        ll du = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (du != dist[u]) continue;
        for (auto e : g[u]) {
            int v = e.first;
            ll w = e.second;
            if (dist[v] > du + w) {
                dist[v] = du + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// 3. Fenwick: point addition + prefix/range sum; indices MUST be 1..n.
struct Fenwick {
    int n;
    vector<ll> bit;
    Fenwick(int n_) : n(n_), bit(n_ + 1, 0) {}
    void add(int i, ll delta) {
        for (; i <= n; i += i & -i) bit[i] += delta;
    }
    ll sum(int i) const {
        ll res = 0;
        for (; i > 0; i -= i & -i) res += bit[i];
        return res;
    }
    ll rangeSum(int l, int r) const { return sum(r) - sum(l - 1); }
};

// 4. Topological ordering: directed graph only; if size<n, graph has a cycle.
vector<int> topoOrder(int n, const vector<vector<int>>& g) {
    vector<int> indeg(n + 1, 0), order;
    for (int u = 1; u <= n; ++u)
        for (int v : g[u]) ++indeg[v];
    queue<int> q;
    for (int u = 1; u <= n; ++u)
        if (indeg[u] == 0) q.push(u);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : g[u])
            if (--indeg[v] == 0) q.push(v);
    }
    return order;
}

// 5. KMP prefix function: pi[i] is the longest proper prefix of s[0..i]
// which is also a suffix of s[0..i]. Indexing is 0-based.
vector<int> prefixFunction(const string& s) {
    vector<int> pi(s.size(), 0);
    for (int i = 1; i < (int)s.size(); ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

// 6. Manacher: length of the LONGEST CONTIGUOUS palindromic substring.
// Valid if original input does NOT contain any of '@', '#', '$'.
// For typical lowercase-letter strings this is fine.
int longestPalSubstring(const string& s) {
    string t = "@#";
    for (char c : s) { t += c; t += '#'; }
    t += '$';
    vector<int> p(t.size(), 0);
    int center = 0, right = 0, best = 0;
    for (int i = 1; i + 1 < (int)t.size(); ++i) {
        if (i < right) p[i] = min(right - i, p[2 * center - i]);
        while (t[i - p[i] - 1] == t[i + p[i] + 1]) ++p[i];
        if (i + p[i] > right) {
            center = i;
            right = i + p[i];
        }
        best = max(best, p[i]);
    }
    return best;
}

// 7. Modular exponentiation: a^e mod mod, e>=0 and 1<=mod<=2e9.
ll modPow(ll a, ll e, ll mod) {
    a = (a % mod + mod) % mod;
    ll ans = 1 % mod;
    while (e > 0) {
        if (e & 1) ans = ans * a % mod;
        a = a * a % mod;
        e >>= 1;
    }
    return ans;
}
