#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;
int vis[N], ans[N];
set<int> ts[2];

int main() {
    int n, p, k;
    cin >> n >> p >> k;
    int m;
    cin >> m;
    for (int i = 1; i <= m; ++i) {
        int b;
        cin >> b;
        vis[b] = 1;
    }

    for (int i = 1; i <= n; ++i) {
        ans[i] = -1;
        if (i != p && !vis[i]) ts[i & 1].insert(i);
    }
    ans[p] = 0;

    queue<int> q;
    q.push(p);
    while (!q.empty()) {
        int i = q.front();
        q.pop();
        int mi = max(i - k + 1, k - i + 1);
        int mx = min(i + k - 1, 2 * n - k - i + 1);
        if (mi > mx) continue;

        set<int> &pos = ts[mi & 1];
        auto it = pos.lower_bound(mi);
        while (it != pos.end() && *it <= mx) {
            int j = *it;
            ans[j] = ans[i] + 1;
            q.push(j);
            it = pos.erase(it);
        }
    }

    for (int i = 1; i <= n; ++i) cout << ans[i] << '\n';
    return 0;
}
