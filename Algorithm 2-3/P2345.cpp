/**
 * Luogu P2345 - [USACO04OPEN] MooFest G
 *
 * 问题描述：
 *   每头奶牛有音量 v 和位置 x。任意两头奶牛之间的贡献为
 *       max(v[i], v[j]) * |x[i] - x[j]|，
 *   求所有奶牛对贡献的总和。
 *
 * 解法：枚举所有无序奶牛对
 *   枚举满足 i < j 的每一对奶牛，直接按题意计算该对贡献并累加。
 *   条件 i < j 使每一对奶牛只计算一次。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n^2)
 *   - 空间复杂度：O(n)，用于保存每头奶牛的音量和位置
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector v(n, 0), x(n, 0);
    for (int i = 0; i < n; i++) cin >> v[i] >> x[i];

    long long sum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            sum += max(v[i], v[j]) * abs(x[i] - x[j]);
        }
    }
    cout << sum << endl;
    return 0;
}
