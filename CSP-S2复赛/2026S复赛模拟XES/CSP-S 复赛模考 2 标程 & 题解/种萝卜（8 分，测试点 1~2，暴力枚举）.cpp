#include <bits/stdc++.h>
using namespace std;

const int N = 10;

int n, h[N];
long long a[N][N], ans;

void calc() {
    long long res = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (i >= h[j] &&
                ((j && i < h[j - 1]) || (j + 1 < n && i < h[j + 1])))
                res += a[i][j];
    ans = max(ans, res);
}

void dfs(int j) {
    if (j > n) {
        calc();
        return;
    }
    for (h[j] = 0; h[j] <= n; ++h[j])
        dfs(j + 1);
}

int main() {
    cin >> n;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> a[i][j];
    dfs(0);
    cout << ans << '\n';
    return 0;
}
