#include <bits/stdc++.h>
using namespace std;

const int N = 10005;

char s[N], t[N], a[2 * N], c[2 * N];

int main() {
    cin >> s >> t;
    int n = strlen(s), m = strlen(t), l = n + m - 1;

    memset(c, 0, l);
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

    for (int i = 0; i < l; ++i) a[i] = c[i] ? c[i] : 'a';
    for (int i = 0; i < n; ++i) {
        if (s[i] != '0') continue;
        bool same = true;
        for (int j = i; j < i + m; ++j)
            if (a[j] != t[j - i]) {
                same = false;
                break;
            }
        if (!same) continue;
        int p = -1;
        for (int j = i + m - 1; j >= i; --j)
            if (!c[j]) {
                p = j;
                break;
            }
        if (p == -1) {
            cout << "-1\n";
            return 0;
        }
        a[p] = 'b';
    }

    a[l] = 0;
    cout << a << '\n';
    return 0;
}
