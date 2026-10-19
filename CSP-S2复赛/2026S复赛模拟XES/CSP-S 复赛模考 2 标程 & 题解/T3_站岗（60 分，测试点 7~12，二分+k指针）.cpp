#include <bits/stdc++.h>
using namespace std;

// 60 分：二分答案 + 朴素 k 指针（枚举每个起点，内层 k 个指针单调移动）。
// check(low) 复杂度 O(n*k)：枚举起点 idx[0] 共 O(n) 次，每次调整 k 个指针。
// 适用于测试点 7~12：k <= 25（n 任意，O(n*k) <= 5e6）。

int n, k;
long long a;
vector<long long> pos;

bool check(long long low) {
    long long L = 4LL * a;
    vector<int> idx(k, 0);
    while (true) {
        // 内层 k 个指针：贪心找「距离 >= low」的下一个点
        bool fail = false;
        for (int j = 1; j < k; ++j) {
            while (idx[j] < n && pos[idx[j]] < pos[idx[j - 1]] + low) ++idx[j];
            if (idx[j] >= n) return false;
        }
        if (pos[idx[k - 1]] - pos[idx[0]] <= L - low) return true;
        // 枚举下一个起点
        ++idx[0];
        if (idx[0] >= n) return false;
    }
}

int main() {
    cin >> n >> k >> a;
    pos.resize(n);
    for (int i = 0; i < n; ++i) {
        long long x, y;
        cin >> x >> y;
        long long p;
        if (x == 0) p = y;
        else if (y == a) p = a + x;
        else if (x == a) p = 3LL * a - y;
        else p = 4LL * a - x;
        pos[i] = p;
    }
    sort(pos.begin(), pos.end());

    long long lo = 1, hi = 4LL * a / k + 1;
    while (lo < hi) {
        long long mid = (lo + hi + 1) / 2;
        if (check(mid)) lo = mid;
        else hi = mid - 1;
    }
    cout << lo << '\n';
    return 0;
}
