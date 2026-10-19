#include <bits/stdc++.h>
using namespace std;

// 100 分标解：二分答案 + DFS（建树 + 树上找 k 个点）。
//
// 关键转化：正方形边界上的点，按顺时针映射到一维周长坐标，问题变成：
//   在环形数组上选 k 个数，任意相邻两个数（含首尾）距离 >= low。
// 二分答案 low，check(low) 用双指针建树 + DFS 判断是否可行。
//
// 映射（周长 4*a）：
//   左边界 x=0         -> y
//   上边界 y=a         -> a + x
//   右边界 x=a         -> 3*a - y
//   下边界 y=0         -> 4*a - x

int n, k;
long long a;
vector<long long> pos; // 一维坐标（已排序），末尾加哨兵

bool check(long long low) {
    // 双指针建树：i 的父节点 j = 满足 pos[j] >= pos[i]+low 的最小下标（不存在则为哨兵 n）
    vector<vector<int>> g(n + 1);
    int j = n;
    for (int i = n - 1; i >= 0; --i) {
        while (j - 1 >= 0 && pos[j - 1] >= pos[i] + low) --j;
        g[j].push_back(i);
    }

    // 迭代 DFS：维护从根到当前节点的路径（pos 值），当路径上点数 >= k 时判断环形首尾距离。
    vector<long long> path;
    vector<pair<int, int>> st; // (节点, 下一个子节点下标)
    st.push_back({n, 0});
    path.push_back(pos[n]); // 哨兵

    long long L = 4LL * a; // 周长
    while (!st.empty()) {
        int x = st.back().first;
        if ((int)path.size() > k && path[path.size() - k] - pos[x] <= L - low) {
            return true;
        }
        int &idx = st.back().second;
        if (idx < (int)g[x].size()) {
            int y = g[x][idx++];
            st.push_back({y, 0});
            path.push_back(pos[y]);
        } else {
            st.pop_back();
            path.pop_back();
        }
    }
    return false;
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
    pos.push_back(4LL * a + 1); // 哨兵，作为根，其值足够大

    long long lo = 1, hi = 4LL * a / k + 1;
    while (lo < hi) {
        long long mid = (lo + hi + 1) / 2;
        if (check(mid)) lo = mid;
        else hi = mid - 1;
    }
    cout << lo << '\n';
    return 0;
}
