#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    vector<int> lg(n + 1);
    for (int i = 2; i <= n; ++i) lg[i] = lg[i / 2] + 1;
    int K = lg[n];
    vector<vector<int>> st(n + 1, vector<int>(K + 1));
    for (int i = 1; i <= n; ++i) st[i][0] = a[i];

    for (int k = 1; k <= K; ++k)
        for (int i = 1; i + (1 << k) - 1 <= n; ++i)
            st[i][k] = max(st[i][k - 1], st[i + (1 << (k - 1))][k - 1]);

    while (q--) {
        int l, r;
        cin >> l >> r;
        int k = lg[r - l + 1];
        cout << max(st[l][k], st[r - (1 << k) + 1][k]) << '\n';
    }
    return 0;
}