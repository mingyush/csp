#include <bits/stdc++.h>
using namespace std;

// 100 分：逆向思维。正向记录每个操作后的长度，逆向倒推 k 所在位置。
//
// 四种操作（从左到右依次读取）：
//   小写字母 c：写入到末尾
//   '-'        ：擦除末尾一个字符（空串则不变）
//   '*'        ：把当前串复制一份接到末尾（长度翻倍）
//   '!'        ：反转当前串
//
// 关键：结果串长度可能指数级增长（'*' 复制），不能真的构造它。
// 只需正向记录每个操作后的长度 size[i]，再从后往前倒推 k 的位置：
//   字母：若 k == size[i]（该字母位于末尾），答案就是它
//   '-' ：k 一定不指向被擦除字符，什么都不做
//   '*' ：若 k > size[i]/2（k 在复制出的后半段），映射回前半 k -= size[i]/2
//   '!' ：k = size[i] - k + 1（反转镜像）
// 题目保证过程中字符串长度不超过 1e18，64 位整数足够。
// 复杂度 O(|s|)。

using LL = long long;
const int N = 2e5 + 5;

LL sz[N];

int main() {
    string s;
    LL k;
    cin >> s >> k; // k 从 1 开始（1-indexed）

    int n = s.size();
    long long cur = 0;
    for (int i = 0; i < n; ++i) {
        char c = s[i];
        if (c == '-') {
            if (cur > 0) cur--;
        } else if (c == '*') {
            cur *= 2;
        } else if (c == '!') {
            // 长度不变
        } else { // 字母
            cur++;
        }
        sz[i] = cur;
    }

    if (k > cur) {
        cout << ".\n";
        return 0;
    }

    for (int i = n - 1; i >= 0; --i) {
        char c = s[i];
        long long L = sz[i];
        if (c == '-') {
            // 什么都不做
        } else if (c == '*') {
            if (k > L / 2) k -= L / 2;
        } else if (c == '!') {
            k = L - k + 1;
        } else { // 字母
            if (k == L) {
                cout << c << '\n';
                return 0;
            }
        }
    }
    cout << ".\n";
    return 0;
}
