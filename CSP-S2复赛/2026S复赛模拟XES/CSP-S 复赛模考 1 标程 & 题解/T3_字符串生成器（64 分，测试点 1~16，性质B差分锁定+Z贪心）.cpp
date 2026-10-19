#include <bits/stdc++.h>
using namespace std;

const int N = 1000005;
const int M = 3000005;

char s[N], t[N], a[M], c[M], q[M];
int z[M], fa[M], d[M];

void getz(char *s, int n, int *z) {
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
}

int find(int x) {
    return fa[x] == x ? x : fa[x] = find(fa[x]);
}

int main() {
    cin >> s >> t;
    int n = strlen(s), m = strlen(t), l = n + m - 1;

    memset(c, 0, l);
    bool ok = true;
    for (int i = 0; i < m; ++i)
        if (t[i] != 'a') ok = false;
    if (ok) {
        for (int i = 0; i < n; ++i)
            if (s[i] == '1') ++d[i], --d[i + m];
        for (int i = 0, x = 0; i < l; ++i) {
            x += d[i];
            if (x) c[i] = 'a';
        }
    } else {
        for (int i = 0; i < n; ++i) {
            if (s[i] != '1') continue;
            for (int j = i; j < i + m; ++j) {
                if (!c[j]) c[j] = t[j - i];
                else if (c[j] != t[j - i]) {
                    cout << "-1\n";
                    return 0;
                }
            }
        }
    }

    for (int i = 0; i < l; ++i) a[i] = c[i] ? c[i] : 'a';
    int qn = 0;
    for (int i = 0; i < m; ++i) q[qn++] = t[i];
    q[qn++] = '#';
    for (int i = 0; i < l; ++i) q[qn++] = a[i];
    getz(q, qn, z);

    for (int i = 0; i <= l; ++i) fa[i] = i;
    for (int i = 0; i < l; ++i)
        if (c[i]) fa[i + 1] = find(i);

    int last = -1;
    for (int i = 0; i < n; ++i) {
        if (s[i] != '0') continue;
        if (z[m + 1 + i] < m) continue;
        if (last >= i) continue;
        int p = find(i + m);
        if (p == 0 || p - 1 < i) {
            cout << "-1\n";
            return 0;
        }
        --p;
        a[p] = 'b';
        fa[p + 1] = find(p);
        last = max(last, p);
    }

    a[l] = 0;
    cout << a << '\n';
    return 0;
}
