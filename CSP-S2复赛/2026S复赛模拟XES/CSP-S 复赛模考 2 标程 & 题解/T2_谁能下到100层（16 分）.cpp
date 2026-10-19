#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;

int n, d, dp[N];
long long h[N];

int dfs(int i) {
	int res = 1;

    for (int j = i - 1; j >= max(1, i - d) && h[j] < h[i]; --j)
        res = max(res, dfs(j) + 1);
    for (int j = i + 1; j <= min(n, i + d) && h[j] < h[i]; ++j)
        res = max(res, dfs(j) + 1);

    return res;
}

int main() {
    cin >> n >> d;
    for (int i = 1; i <= n; ++i)
        cin >> h[i];

    int ans = 0;
    for (int i = 1; i <= n; ++i)
        ans = max(ans, dfs(i));

    cout << ans << '\n';
    return 0;
}
