#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    long long cur = a[1];
    long long ans = a[1];   // 故意改成 0，可测试对拍能否抓到全负数反例

    for (int i = 2; i <= n; ++i) {
        cur = max(a[i], cur + a[i]);
        ans = max(ans, cur);
    }
    cout << ans << '\n';
    return 0;
}
