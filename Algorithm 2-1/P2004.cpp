/**
 * Luogu P2004 - 领地选择
 *
 * 问题描述：
 *   在一个 n × m 的矩阵中，每个格子有一个价值。
 *   需要选择一个 c × c 的子矩阵（领地），使得该子矩阵内所有格子的价值之和最大。
 *   输出该子矩阵左上角格子的坐标（1-based）。
 *
 * 解法：滑动窗口（双层）
 *   - 暴力枚举所有 c × c 子矩阵需 O(n·m·c²)，数据大时不可接受。
 *   - 利用滑动窗口把枚举优化为 O(n·m·c)：
 *     1) 外层枚举子矩阵的左上角行 i (0 ≤ i + c - 1 < n)；
 *     2) 内层沿列方向滑动窗口：
 *        - 起点 j = 0 时，直接累加 c 行 × c 列得到 sum；
 *        - 每次右移一列：扣除左侧离开的列、补上右侧新进入的列，
 *          即 sum += v[i+k][j+c-1] - v[i+k][j-1]，无需重新遍历 c 列。
 *   - 滑动过程中维护 maxSum 及对应左上角坐标 (x, y)。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n·m·c)
 *   - 空间复杂度：O(n·m)
 *   - 当 c 较大时，可进一步用二维前缀和优化到 O(n·m)
 */

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int main() {
    int n, m, c;
    cin >> n >> m >> c;  // n: 行数, m: 列数, c: 领地边长
    // v: 输入的 n × m 矩阵
    vector v(n, vector(m, 0));
    // 读入矩阵
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> v[i][j];
        }
    }

    // x, y: 当前最大和子矩阵的左上角坐标（0-based）
    // maxSum: 当前已知的最大 c × c 子矩阵元素和
    long long x = 0, y = 0, maxSum = LONG_LONG_MIN;
    // 外层：枚举子矩阵左上角的行坐标 i
    for (int i = 0; i + c - 1 < n; i++) {
        int sum = 0;
        // 起点 j = 0：直接累加 c 行 × c 列的初始窗口
        for (int k = 0; k < c; k++) {
            for (int t = 0; t < c; t++) {
                sum += v[i + k][t];
            }
        }
        if (sum > maxSum) {
            maxSum = sum;
            x = i;
            y = 0;  // 此时子矩阵的左上角列坐标为 0
        }

        // 内层：沿列方向滑动窗口，每次右移一列
        for (int j = 1; j + c - 1 < m; j++) {
            // 滑动更新：每行加新进列 v[i+k][j+c-1]，减离开列 v[i+k][j-1]
            for (int k = 0; k < c; k++) {
                sum += v[i + k][j + c - 1] - v[i + k][j - 1];
            }
            if (sum > maxSum) {
                maxSum = sum;
                x = i;
                y = j;  // 记录当前最优子矩阵的左上角列坐标
            }
        }
    }
    // 输出 1-based 坐标
    cout << x + 1 << " " << y + 1 << endl;
    return 0;
}
