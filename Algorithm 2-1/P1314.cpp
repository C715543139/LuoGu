/**
 * Luogu P1314 - [NOIP 2011 提高组] 聪明的质监员
 *
 * 问题描述：
 *   给定 m 个物品，每个物品有重量 w[i] 和价值 v[i]；
 *   给定 n 个区间 [l[i], r[i]]，需要选取一个阈值 W。
 *   当物品重量 w[i] >= W 时，物品"被选中"，区间 [l, r] 的收益 y 定义为：
 *       y = (区间内被选中的物品数量) * (区间内被选中的物品价值之和)
 *   求使 |s - y| 最小的 W 对应的 |s - y| 值。
 *
 * 解法：二分 + 前缀和
 *   - 阈值 W 越大，被选中的物品越少，y 越小（y 关于 W 单调递减）
 *   - 因此可用二分搜索最优的 W
 *   - 每次二分时，通过前缀和 O(1) 计算每个区间内的"被选数量"与"被选价值之和"
 *   - 时间复杂度：O(log(MaxW) * (m + n))，M = 2000000
 */

#include <iostream>
#include <climits>
#include <vector>
using namespace std;

int main() {
    long long m, n, s;
    cin >> m >> n >> s;  // m: 物品数量, n: 区间数量, s: 目标值
    vector<int> w(m), v(m), l(n), r(n);

    // 读入每个物品的重量 w[i] 与价值 v[i]
    for (int i = 0; i < m; i++) {
        cin >> w[i] >> v[i];
    }
    // 读入每个区间的左右端点 [l[i], r[i]]
    for (int i = 0; i < n; i++) {
        cin >> l[i] >> r[i];
    }

    // 二分搜索阈值 W
    // lW: 搜索区间左端, rW: 搜索区间右端, minSub: 最小 |s - y|
    long long lW = 1, rW = 2000001, minSub = LONG_LONG_MAX;
    while (lW <= rW) {
        long long W = (lW + rW) / 2;  // 候选阈值

        // 构建前缀和：
        // sumW[i]: 前 i 个物品中"被选中"的数量（即 w[j] >= W 的个数）
        // sumV[i]: 前 i 个物品中"被选中"的价值之和
        vector sumW(m + 1, 0), sumV(m + 1, 0);
        for (int i = 1; i <= m; i++) {
            if (w[i - 1] >= W) {
                // 当前物品被选中
                sumW[i] = sumW[i - 1] + 1;
                sumV[i] = sumV[i - 1] + v[i - 1];
            } else {
                // 当前物品未被选中
                sumW[i] = sumW[i - 1];
                sumV[i] = sumV[i - 1];
            }
        }

        // 计算当前阈值下，所有区间的总收益 y
        long long y = 0;
        for (int i = 0; i < n; i++) {
            int left = l[i], right = r[i];
            // 区间 [left, right] 内的被选数量 × 被选价值之和
            y += (sumW[right] - sumW[left - 1]) * (sumV[right] - sumV[left - 1]);
        }
        // 更新最小差值
        minSub = min(minSub, abs(s - y));

        // 根据 y 与 s 的关系调整二分区间
        // y 关于 W 单调递减：W 越大，被选物品越少，y 越小
        if (s > y) {
            // 当前 y 偏小，需要增大 y → 减小 W
            rW = W - 1;
        } else {
            // 当前 y 偏大或相等，需要减小 y → 增大 W
            lW = W + 1;
        }
    }
    cout << minSub << endl;
    return 0;
}
