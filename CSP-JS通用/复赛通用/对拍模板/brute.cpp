#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    long long ans = LLONG_MIN;
    for (int l = 1; l <= n; ++l) {
        long long sum = 0;
        for (int r = l; r <= n; ++r) {
            sum += a[r];
            ans = max(ans, sum);
        }
    }
    cout << ans << '\n';
    return 0;
}
