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

int n, lg[N], st[N];
long long k, a[N], pre[N];
long long mx[K][N], mn[K][N];
int lmx[N], rmx[N], lmn[N], rmn[N];

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

long long solveall() {
    int top = 0;
    for (int i = 1; i <= n; ++i) {
        while (top && a[st[top]] < a[i]) --top;
        lmx[i] = top ? st[top] : 0;
        st[++top] = i;
    }
    top = 0;
    for (int i = n; i >= 1; --i) {
        while (top && a[st[top]] <= a[i]) --top;
        rmx[i] = top ? st[top] : n + 1;
        st[++top] = i;
    }
    top = 0;
    for (int i = 1; i <= n; ++i) {
        while (top && a[st[top]] > a[i]) --top;
        lmn[i] = top ? st[top] : 0;
        st[++top] = i;
    }
    top = 0;
    for (int i = n; i >= 1; --i) {
        while (top && a[st[top]] >= a[i]) --top;
        rmn[i] = top ? st[top] : n + 1;
        st[++top] = i;
    }

    long long ans = 0;
    for (int i = 1; i <= n; ++i) {
        long long c1 = 1LL * (i - lmx[i]) * (rmx[i] - i);
        long long c2 = 1LL * (i - lmn[i]) * (rmn[i] - i);
        ans += a[i] * (c1 - c2);
    }
    return ans;
}

long long cntge(long long x) {
    long long cnt = 0;
    int l = 0;
    for (int r = 1; r <= n; ++r) {
        while (l + 1 <= r && a[r] - a[l + 1] >= x) ++l;
        cnt += l;
    }
    return cnt;
}

long long sumge(long long x) {
    long long ans = 0;
    int l = 0;
    for (int r = 1; r <= n; ++r) {
        while (l + 1 <= r && a[r] - a[l + 1] >= x) ++l;
        if (l) ans += 1LL * l * a[r] - pre[l];
    }
    return ans;
}

long long solvest() {
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
    return ans;
}

long long solveinc() {
    for (int i = 1; i <= n; ++i) pre[i] = pre[i - 1] + a[i];
    long long lo = 0, hi = a[n] - a[1];
    while (lo < hi) {
        long long mid = (lo + hi + 1) >> 1;
        if (cntge(mid) >= k) lo = mid;
        else hi = mid - 1;
    }
    long long v = lo;
    long long c = cntge(v + 1), s = sumge(v + 1);
    return s + v * (k - c);
}

int main() {
    cin >> n >> k;
    for (int i = 1; i <= n; ++i) cin >> a[i];

    bool ok = true;
    for (int i = 2; i <= n; ++i)
        if (a[i] < a[i - 1]) ok = false;
    if (ok) cout << solveinc() << '\n';
    else if (k == 1LL * n * (n + 1) / 2) cout << solveall() << '\n';
    else cout << solvest() << '\n';
    return 0;
}
