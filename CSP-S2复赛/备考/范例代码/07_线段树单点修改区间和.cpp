#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

struct SegTree {
    int n;
    vector<ll> tr;
    SegTree(int n): n(n), tr(4 * n + 4) {}

    void build(int p, int l, int r, const vector<ll>& a) {
        if (l == r) { tr[p] = a[l]; return; }
        int m = (l + r) / 2;
        build(p*2, l, m, a);
        build(p*2+1, m+1, r, a);
        tr[p] = tr[p*2] + tr[p*2+1];
    }

    void update(int p, int l, int r, int x, ll v) {
        if (l == r) { tr[p] = v; return; }
        int m = (l + r) / 2;
        if (x <= m) update(p*2, l, m, x, v);
        else update(p*2+1, m+1, r, x, v);
        tr[p] = tr[p*2] + tr[p*2+1];
    }

    ll query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p];
        int m = (l + r) / 2;
        ll ans = 0;
        if (ql <= m) ans += query(p*2, l, m, ql, qr);
        if (qr > m) ans += query(p*2+1, m+1, r, ql, qr);
        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<ll> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];
    SegTree st(n); st.build(1, 1, n, a);
    while (q--) {
        char op; int x, y; cin >> op >> x >> y;
        if (op == 'U') st.update(1, 1, n, x, y);
        else cout << st.query(1, 1, n, x, y) << '\n';
    }
}