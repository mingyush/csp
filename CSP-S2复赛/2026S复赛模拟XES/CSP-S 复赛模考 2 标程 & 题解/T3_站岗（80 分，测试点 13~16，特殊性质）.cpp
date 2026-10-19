#include <bits/stdc++.h>
using namespace std;

// 80 分：特殊性质 A（所有点在同一条边上）+ 特殊性质 B（k = n）。
//
// 性质 B（k = n）：选全部点，答案 = 环形相邻两点距离的最小值，O(n) 直接算。
//
// 性质 A（所有点在同一条边上）：点映射到一维后集中在周长的一段，
//   且 k >= 4 时 low <= a，而环形首尾距离 >= 3a，必然 >= low，
//   故「环形」退化成「线性」，只需线性贪心 O(n)（无需正解的 DFS 建树）。
//
// 其他情况（无性质）：退回到 O(n*k) 的「枚举起点 + k 个指针」（即 60 分做法），
//   在 k <= 25 的测试点（7~12）能过，但在 k 很大（17~20）时会超时，
//   从而拿不到 100 分。

int n, k;
long long a;
vector<long long> pos;

int main() {
    cin >> n >> k >> a;
    vector<long long> xs(n), ys(n);
    pos.resize(n);
    for (int i = 0; i < n; ++i) {
        long long x, y;
        cin >> x >> y;
        xs[i] = x; ys[i] = y;
        long long p;
        if (x == 0) p = y;
        else if (y == a) p = a + x;
        else if (x == a) p = 3LL * a - y;
        else p = 4LL * a - x;
        pos[i] = p;
    }
    sort(pos.begin(), pos.end());
    long long L = 4LL * a;

    // 性质 B：k == n
    if (k == n) {
        long long ans = LLONG_MAX;
        for (int i = 0; i + 1 < n; ++i) ans = min(ans, pos[i + 1] - pos[i]);
        ans = min(ans, pos[0] + L - pos[n - 1]);
        cout << ans << '\n';
        return 0;
    }

    // 性质 A：所有点在同一条边上
    bool sameEdge = true;
    {
        bool allLeft = true, allRight = true, allDown = true, allUp = true;
        for (int i = 0; i < n; ++i) {
            if (xs[i] != 0) allLeft = false;
            if (xs[i] != a) allRight = false;
            if (ys[i] != 0) allDown = false;
            if (ys[i] != a) allUp = false;
        }
        sameEdge = allLeft || allRight || allDown || allUp;
    }

    long long lo = 1, hi = L / k + 1;
    if (sameEdge) {
        // 线性贪心（首尾自动满足，无需考虑环形）
        auto check = [&](long long low) -> bool {
            int cnt = 1;
            long long last = pos[0];
            for (int i = 1; i < n; ++i) {
                if (pos[i] - last >= low) {
                    ++cnt;
                    last = pos[i];
                    if (cnt == k) return true;
                }
            }
            return false;
        };
        while (lo < hi) {
            long long mid = (lo + hi + 1) / 2;
            if (check(mid)) lo = mid;
            else hi = mid - 1;
        }
    } else {
        // O(n*k) 枚举起点 + k 个指针（含环形首尾），k 小时快、k 大时超时
        auto check = [&](long long low) -> bool {
            vector<int> idx(k, 0);
            while (true) {
                bool fail = false;
                for (int j = 1; j < k; ++j) {
                    while (idx[j] < n && pos[idx[j]] < pos[idx[j - 1]] + low) ++idx[j];
                    if (idx[j] >= n) return false;
                }
                if (pos[idx[k - 1]] - pos[idx[0]] <= L - low) return true;
                ++idx[0];
                if (idx[0] >= n) return false;
            }
        };
        while (lo < hi) {
            long long mid = (lo + hi + 1) / 2;
            if (check(mid)) lo = mid;
            else hi = mid - 1;
        }
    }
    cout << lo << '\n';
    return 0;
}
