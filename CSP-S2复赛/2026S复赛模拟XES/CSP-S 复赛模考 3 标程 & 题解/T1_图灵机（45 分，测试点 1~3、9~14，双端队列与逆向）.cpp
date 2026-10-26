#include <bits/stdc++.h>
using namespace std;

using LL = long long;
const int N = 2e5 + 5;

LL sz[N];

char Calc(string s, LL k) {
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
    if (k > t.size()) return '.';
    return t[k - 1];
}

int main() {
    string s;
    LL k;
    cin >> s >> k;

    int n = s.size();
    if (n <= 10) {
        cout << Calc(s, k) << '\n';
        return 0;
    }

    if (s.find('*') == string::npos) {
        deque<char> t;
        bool rev = false;
        for (char c : s) {
            if (c == '!') {
                rev = !rev;
            } else if (c == '-') {
                if (!t.empty()) {
                    if (rev) t.pop_front();
                    else t.pop_back();
                }
            } else {
                if (rev) t.push_front(c);
                else t.push_back(c);
            }
        }
        if (k > (LL)t.size()) cout << ".\n";
        else if (rev) cout << t[t.size() - k] << '\n';
        else cout << t[k - 1] << '\n';
        return 0;
    }

    if (s.find('!') == string::npos) {
        LL cur = 0;
        for (int i = 0; i < n; ++i) {
            char c = s[i];
            if (c == '-') {
                if (cur > 0) cur--;
            } else if (c == '*') {
                cur *= 2;
            } else {
                cur++;
            }
            sz[i] = cur;
        }

        if (k > cur) {
            cout << ".\n";
            return 0;
        }
        for (int i = n - 1; i >= 0; --i) {
            char c = s[i];
            LL len = sz[i];
            if (c == '-') {
                continue;
            } else if (c == '*') {
                if (k > len / 2) k -= len / 2;
            } else if (k == len) {
                cout << c << '\n';
                return 0;
            }
        }
        cout << ".\n";
        return 0;
    }

    cout << ".\n";
    return 0;
}
