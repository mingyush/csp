#include <bits/stdc++.h>
using namespace std;

const int N = 200005;
const long long INF = 4000000000000000000LL;

int n;
long long k, a[N];
int qmx[N], qmn[N];

long long mxv[N], mnv[N];
int mxl[N], mnl[N];
int topm, topn;

struct Seg {
    long long mn[N << 2], lz[N << 2];
    long long sum[N << 2];

    void up(int p) {
        mn[p] = min(mn[p << 1], mn[p << 1 | 1]);
        sum[p] = sum[p << 1] + sum[p << 1 | 1];
    }

    void build(int p, int l, int r) {
        lz[p] = 0;
        if (l == r) {
            mn[p] = INF;
            sum[p] = INF;
            return;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid);
        build(p << 1 | 1, mid + 1, r);
        up(p);
    }

    void down(int p, int l, int r) {
        if (!lz[p] || l == r) return;
        int mid = (l + r) >> 1;
        long long x = lz[p];
        mn[p << 1] += x;
        mn[p << 1 | 1] += x;
        lz[p << 1] += x;
        lz[p << 1 | 1] += x;
        sum[p << 1] += 1LL * (mid - l + 1) * x;
        sum[p << 1 | 1] += 1LL * (r - mid) * x;
        lz[p] = 0;
    }

    void add(int p, int l, int r, int x, int y, long long v) {
        if (x > y || x > r || y < l) return;
        if (x <= l && r <= y) {
            mn[p] += v;
            lz[p] += v;
            sum[p] += 1LL * (r - l + 1) * v;
            return;
        }
        down(p, l, r);
        int mid = (l + r) >> 1;
        add(p << 1, l, mid, x, y, v);
        add(p << 1 | 1, mid + 1, r, x, y, v);
        up(p);
    }

    void add(int l, int r, long long v) {
        if (l <= r) add(1, 1, n, l, r, v);
    }

    void set0(int p, int l, int r, int x) {
        if (l == r) {
            mn[p] = 0;
            lz[p] = 0;
            sum[p] = 0;
            return;
        }
        down(p, l, r);
        int mid = (l + r) >> 1;
        if (x <= mid) set0(p << 1, l, mid, x);
        else set0(p << 1 | 1, mid + 1, r, x);
        up(p);
    }

    void set0(int x) {
        set0(1, 1, n, x);
    }

    int first(int p, int l, int r, int R, long long x) {
        if (l > R || mn[p] >= x) return R + 1;
        if (l == r) return l;
        down(p, l, r);
        int mid = (l + r) >> 1;
        int z = first(p << 1, l, mid, R, x);
        if (z <= R) return z;
        return first(p << 1 | 1, mid + 1, r, R, x);
    }

    int first(int R, long long x) {
        return first(1, 1, n, R, x);
    }

    long long ask(int p, int l, int r, int x, int y) {
        if (x > r || y < l || x > y) return 0;
        if (x <= l && r <= y) return sum[p];
        down(p, l, r);
        int mid = (l + r) >> 1;
        return ask(p << 1, l, mid, x, y)
             + ask(p << 1 | 1, mid + 1, r, x, y);
    }

    long long ask(int l, int r) {
        if (l > r) return 0;
        return ask(1, 1, n, l, r);
    }
} tr;

long long cnt(long long x) {
    int l = 1, hm = 1, rm = 0, hn = 1, rn = 0;
    long long res = 0;

    for (int r = 1; r <= n; ++r) {
        while (hm <= rm && a[qmx[rm]] <= a[r]) --rm;
        qmx[++rm] = r;

        while (hn <= rn && a[qmn[rn]] >= a[r]) --rn;
        qmn[++rn] = r;

        while (l <= r && a[qmx[hm]] - a[qmn[hn]] >= x) {
            ++l;
            while (hm <= rm && qmx[hm] < l) ++hm;
            while (hn <= rn && qmn[hn] < l) ++hn;
        }
        res += l - 1;
    }
    return res;
}

void addmax(int r) {
    long long x = a[r];
    int l = r;
    while (topm && mxv[topm] <= x) {
        tr.add(mxl[topm], l - 1, x - mxv[topm]);
        l = mxl[topm];
        --topm;
    }
    mxv[++topm] = x;
    mxl[topm] = l;
}

void addmin(int r) {
    long long x = a[r];
    int l = r;
    while (topn && mnv[topn] >= x) {
        tr.add(mnl[topn], l - 1, mnv[topn] - x);
        l = mnl[topn];
        --topn;
    }
    mnv[++topn] = x;
    mnl[topn] = l;
}

long long sumge(long long x) {
    tr.build(1, 1, n);
    topm = topn = 0;
    long long res = 0;

    for (int r = 1; r <= n; ++r) {
        tr.set0(r);
        addmax(r);
        addmin(r);

        int p = tr.first(r, x);
        res += tr.ask(1, p - 1);
    }
    return res;
}

int main() {
    cin >> n >> k;
    long long mn = LLONG_MAX, mx = LLONG_MIN;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        mn = min(mn, a[i]);
        mx = max(mx, a[i]);
    }

    long long l = 0, r = mx - mn;
    while (l < r) {
        long long mid = (l + r + 1) >> 1;
        if (cnt(mid) >= k) l = mid;
        else r = mid - 1;
    }

    long long v = l;
    long long c = cnt(v + 1);
    long long ans = sumge(v + 1) + v * (k - c);
    cout << ans << '\n';
    return 0;
}
