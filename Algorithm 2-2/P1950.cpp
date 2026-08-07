/**
 * Luogu P1950 - 长方形
 *
 * 问题描述：
 *   在由 '.' 和 '*' 组成的 n×m 网格中，统计所有仅包含 '.' 的子矩形数量。
 *
 * 解法：逐行直方图 + 单调栈计数
 *   1. 对每一行，h[j] 表示第 j 列从当前行向上连续 '.' 的数量。
 *      当前行可视为以 h 为柱高的直方图，每个全 '.' 矩形都有唯一的底边行。
 *   2. 对每根柱子 j，利用单调栈求：
 *      - l[j]：左侧最近的、柱高小于等于 h[j] 的位置；
 *      - r[j]：右侧最近的、柱高严格小于 h[j] 的位置。
 *      这组不对称边界把相同最小高度的矩形唯一分配给最左侧的最小柱，避免重复计数。
 *   3. 以 j 为最左侧最小柱的矩形：
 *      高度有 h[j] 种选择，左端点有 j-l[j] 种选择，右端点有 r[j]-j 种选择，
 *      因而贡献 h[j] × (j-l[j]) × (r[j]-j) 个矩形。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(nm)，每个位置在每行的两个单调栈中最多入栈、出栈一次
 *   - 空间复杂度：O(m)
 */

#include <iostream>
#include <stack>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    // h[j]：第 j 列以当前行为底、向上连续 '.' 的高度
    vector h(m, 0);
    // 全空矩形数量最大可达 O(n^2m^2)，使用 long long 保存
    long long res = 0;
    // 逐行处理，每一行作为候选矩形的底边
    for (int i = 0; i < n; i++) {
        // v[j]=true 表示当前行第 j 列为空格 '.'
        vector v(m, false);
        // 读入当前行
        for (int j = 0; j < m; j++) {
            char c;
            cin >> c;
            v[j] = c == '.';
        }

        // stL：柱高单调不减，用于寻找左侧最近的高度 <= h[j] 的位置
        // stR：从右向左维护，用于寻找右侧最近的高度 < h[j] 的位置
        stack<pair<int, int>> stL, stR;
        // -1 与 m 分别是左右两侧的虚拟边界
        vector l(m, -1), r(m, m);
        // 从左向右更新柱高并计算左边界
        for (int j = 0; j < m; j++) {
            if (v[j]) {
                // 当前格为空，直方图柱高增加
                h[j]++;

                // 栈顶已不高于当前柱时，它就是最近的左边界
                if (!stL.empty() && stL.top().first <= h[j]) {
                    l[j] = stL.top().second;
                } else {
                    // 弹出更高的柱，保留最近的高度 <= h[j] 的柱
                    while (!stL.empty() && stL.top().first > h[j]) stL.pop();
                    if (!stL.empty()) l[j] = stL.top().second;
                }
            } else {
                // '*' 会切断向上的连续空格高度
                h[j] = 0;
            }
            // 高度为 0 的位置也入栈，作为后续柱子的障碍边界
            stL.emplace(h[j], j);
        }

        // 从右向左计算右边界；对相同高度使用严格小于的边界以避免重复计数
        for (int j = m - 1; j >= 0; j--) {
            if (v[j]) {
                // 栈顶严格更低时，它就是最近的右边界
                if (!stR.empty() && stR.top().first < h[j]) {
                    r[j] = stR.top().second;
                } else {
                    // 弹出高度大于等于 h[j] 的柱，寻找右侧最近严格更低的柱
                    while (!stR.empty() && stR.top().first >= h[j]) stR.pop();
                    if (!stR.empty()) r[j] = stR.top().second;
                }
            }
            // 高度为 0 的位置也入栈，作为后续柱子的障碍边界
            stR.emplace(h[j], j);
        }

        // 累加以当前行为底、以第 j 列为最左侧最小柱的所有全空矩形
        for (int j = 0; j < m; j++) {
            if (v[j]) {
                res += h[j] * (j - l[j]) * (r[j] - j);
            }
        }
    }

    cout << res << endl;
    return 0;
}
