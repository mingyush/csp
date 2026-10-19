#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;

int n, d, dp[N], nl[N], nr[N], id[N];
long long h[N];

bool cmp1(int a, int b) {
    return h[a] < h[b];
}

bool cmp2(int a, int b) {
    return h[a] > h[b];
}

int main() {
    cin >> n >> d;
    for (int i = 1; i <= n; ++i) {
        cin >> h[i];
        id[i] = i;
    }

    bool dec = true;
    for (int i = 2; i <= n; ++i)
        if (h[i] >= h[i - 1]) {
            dec = false;
            break;
        }

    if (dec) {
        cout << n << '\n';
        return 0;
    }

    if (d == n) {
        stack<int> S;
        for (int i = 1; i <= n; ++i) {
            while (!S.empty() && h[S.top()] <= h[i])
                S.pop();
            nl[i] = S.empty() ? 0 : S.top();
            S.push(i);
        }

        while (!S.empty()) S.pop();
        for (int i = n; i >= 1; --i) {
            while (!S.empty() && h[S.top()] <= h[i])
                S.pop();
            nr[i] = S.empty() ? n + 1 : S.top();
            S.push(i);
        }

        sort(id + 1, id + n + 1, cmp2);
        int ans = 0;
        for (int k = 1; k <= n; ++k) {
            int i = id[k];
            dp[i] = 1;
            if (nl[i]) dp[i] = max(dp[i], dp[nl[i]] + 1);
            if (nr[i] <= n) dp[i] = max(dp[i], dp[nr[i]] + 1);
            ans = max(ans, dp[i]);
        }
        cout << ans << '\n';
        return 0;
    }

    sort(id + 1, id + n + 1, cmp1);
    int ans = 0;
    for (int k = 1; k <= n; ++k) {
        int i = id[k];
        dp[i] = 1;
        for (int j = i - 1; j >= max(1, i - d); --j) {
            if (h[j] >= h[i]) break;
            dp[i] = max(dp[i], dp[j] + 1);
        }
        for (int j = i + 1; j <= min(n, i + d); ++j) {
            if (h[j] >= h[i]) break;
            dp[i] = max(dp[i], dp[j] + 1);
        }
        ans = max(ans, dp[i]);
    }

    cout << ans << '\n';
    return 0;
}
