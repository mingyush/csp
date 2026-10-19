#include <bits/stdc++.h>
using namespace std;

const int N = 200005;
const int K = 20;

struct node {
    long long v;
    int l, r;
    bool operator < (const node &x) const {
        return v < x.v;
    }
};

int n, lg[N];
long long k, a[N], mx[K][N], mn[K][N];

// ST 表查询极差。
void init() {
    for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
    for (int i = 1; i <= n; ++i) mx[0][i] = mn[0][i] = a[i];
    for (int j = 1; j <= lg[n]; ++j)
        for (int i = 1; i + (1 << j) - 1 <= n; ++i) {
            mx[j][i] = max(mx[j - 1][i], mx[j - 1][i + (1 << (j - 1))]);
            mn[j][i] = min(mn[j - 1][i], mn[j - 1][i + (1 << (j - 1))]);
        }
}

long long val(int l, int r) {
    int j = lg[r - l + 1];
    long long x = max(mx[j][l], mx[j][r - (1 << j) + 1]);
    long long y = min(mn[j][l], mn[j][r - (1 << j) + 1]);
    return x - y;
}

int main() {
    cin >> n >> k;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    init();

    priority_queue<node> q;
    for (int l = 1; l <= n; ++l) q.push({val(l, n), l, n});
    long long ans = 0;
    for (long long i = 1; i <= k; ++i) {
        node x = q.top();
        q.pop();
        ans += x.v;
        if (x.l < x.r) q.push({val(x.l, x.r - 1), x.l, x.r - 1});
    }
    cout << ans << '\n';
    return 0;
}
