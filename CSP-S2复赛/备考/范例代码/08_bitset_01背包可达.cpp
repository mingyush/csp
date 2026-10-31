#include <bitset>
#include <iostream>
using namespace std;

const int MAXS = 5000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, target;
    cin >> n >> target;
    bitset<MAXS + 1> f;
    f[0] = 1;

    for (int i = 0; i < n; ++i) {
        int w; cin >> w;
        f |= (f << w);
    }

    cout << (target <= MAXS && f[target] ? "Yes" : "No") << '\n';
    return 0;
}