#include <bits/stdc++.h>
using namespace std;

using LL = long long;

int main() {
    string s;
    LL k;
    cin >> s >> k;

    string t;
    for (char c : s) {
        if (c == '-') {
            if (!t.empty()) t.pop_back();
        } else if (c == '*') {
            t += t;
        } else if (c == '!') {
            reverse(t.begin(), t.end());
        } else {
            t.push_back(c);
        }
    }

    if (k > (LL)t.size()) cout << ".\n";
    else cout << t[k - 1] << '\n';
    return 0;
}
