#include <bits/stdc++.h>
using namespace std;

int n, m;
string s, t;

bool match(int l, int r, int x, int y, vector<int> &v) {
    v.clear();
    int p = l;
    for (int i = x; i <= y; ++i) {
        while (p <= r && s[p] != t[i]) ++p;
        if (p > r) return false;
        v.push_back(p++);
    }
    return true;
}

int main() {
    string a, b;
    cin >> a >> b;
    s = " " + a;
    t = " " + b;
    n = a.size();
    m = b.size();

    vector<int> ans, now, pre, suf;
    if (match(1, n, 1, m, now)) ans = now;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s[i] == t[j]) continue;
            if (!match(1, i - 1, 1, j - 1, pre)) continue;
            if (!match(i + 1, n, j + 1, m, suf)) continue;

            now = pre;
            now.push_back(i);
            now.insert(now.end(), suf.begin(), suf.end());
            if (ans.empty() || now < ans) ans = now;
        }
    }

    if (ans.empty()) {
        cout << "-1\n";
    } else {
        for (int i = 0; i < m; ++i)
            cout << ans[i] << (i + 1 == m ? '\n' : ' ');
    }
    return 0;
}
