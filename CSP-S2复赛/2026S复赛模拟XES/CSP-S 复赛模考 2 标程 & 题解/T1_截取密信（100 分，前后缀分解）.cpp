#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;

int n, m, suf[N], ans[N];
string s, t;

int main() {
    cin >> s >> t;
    n = s.size();
    m = t.size();
    s = " " + s;
    t = " " + t;

    suf[n + 1] = m + 1;
    for (int i = n, j = m; i >= 1; --i) {
        if (j >= 1 && s[i] == t[j]) --j;
        suf[i] = j + 1;
    }

    bool changed = false;
    int j = 1;
    for (int i = 1; i <= n && j <= m; ++i)
        if (s[i] == t[j] || !changed && suf[i + 1] <= j + 1) {
            if (s[i] != t[j]) changed = true;
            ans[j++] = i;
        }

    if (j <= m) cout << "-1\n";
    else
        for (int i = 1; i <= m; ++i)
            cout << ans[i] << " \n"[i == m];
    return 0;
}
