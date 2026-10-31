#include <chrono>
#include <iostream>
#include <random>
using namespace std;

int main() {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    int n = rng() % 8 + 1;
    cout << n << '\n';

    for (int i = 1; i <= n; ++i) {
        int x = (int)(rng() % 19) - 9;  // -9..9
        cout << x << (i == n ? '\n' : ' ');
    }
    return 0;
}
