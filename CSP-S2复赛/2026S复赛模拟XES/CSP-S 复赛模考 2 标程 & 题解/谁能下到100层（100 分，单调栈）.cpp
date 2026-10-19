#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;

int n, d, dp[N], ls[N], rs[N];
long long h[N];

int dfs(int i) {
    if (i == 0)
        return 0;
    if (dp[i])
        return dp[i];
    dp[i] = max(dfs(ls[i]), dfs(rs[i])) + 1;
    return dp[i];
}

int main() {
    cin >> n >> d;
    for (int i = 1; i <= n; ++i)
        cin >> h[i];

    stack<int> S;
    for (int i = 1; i <= n; ++i) {
        while (!S.empty() && h[S.top()] <= h[i])
            S.pop();
        if (!S.empty() && i - S.top() <= d)
            ls[i] = S.top();
        S.push(i);
    }

    while (!S.empty())
        S.pop();
    for (int i = n; i >= 1; --i) {
        while (!S.empty() && h[S.top()] <= h[i])
            S.pop();
        if (!S.empty() && S.top() - i <= d)
            rs[i] = S.top();
        S.push(i);
    }

    int ans = 0;
    for (int i = 1; i <= n; ++i)
        ans = max(ans, dfs(i));

    cout << ans << '\n';
    return 0;
}
