#include <bits/stdc++.h>
using namespace std;

char simulate(string s, long long k) {
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
    if (k > (long long)t.size()) return '.';
    return t[k - 1];
}

int main() {
    string s;
    long long k;
    cin >> s >> k;
    int n = (int)s.size();

    if (n <= 10) {
        cout << simulate(s, k) << '\n';
        return 0;
    }

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

    if (k > (long long)t.size()) {
        cout << ".\n";
    } else if (rev) {
        cout << t[t.size() - k] << '\n';
    } else {
        cout << t[k - 1] << '\n';
    }
    return 0;
}
