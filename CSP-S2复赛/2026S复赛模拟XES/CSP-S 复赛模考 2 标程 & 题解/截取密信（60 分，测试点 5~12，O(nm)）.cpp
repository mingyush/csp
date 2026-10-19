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
    for (int j = 0; j <= m; ++j) {
        if (!match(1, n, 1, j, pre)) continue;

        if (j == m) {
            now = pre;
        } else {
            int p = pre.empty() ? 1 : pre.back() + 1;
            if (p > n) continue;
            now = pre;
            now.push_back(p);
            if (!match(p + 1, n, j + 2, m, suf)) continue;
            now.insert(now.end(), suf.begin(), suf.end());
        }

        if (ans.empty() || now < ans) ans = now;
    }

    if (ans.empty()) {
        cout << "-1\n";
    } else {
        for (int i = 0; i < m; ++i)
            cout << ans[i] << (i + 1 == m ? '\n' : ' ');
    }
    return 0;
}
