/**
 * Luogu P1257 - 平面上的最接近点对
 *
 * 问题描述：
 *   给定平面上的 n 个点，求任意两点之间的最小欧几里得距离。
 *
 * 解法：枚举所有点对
 *   枚举每一对下标满足 i < j 的点，计算其距离的平方
 *       (x[i] - x[j])^2 + (y[i] - y[j])^2，
 *   并维护最小距离平方。最后开平方并保留四位小数输出。
 *   先比较平方距离可以避免在循环中重复计算平方根。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n^2)
 *   - 空间复杂度：O(n)，用于保存所有点的横、纵坐标
 */

#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
#include <iomanip>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector x(n, 0), y(n, 0);
    for (int i = 0; i < n; i++) cin >> x[i] >> y[i];

    long long ans = LONG_LONG_MAX;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            long long d_x = x[i] - x[j], d_y = y[i] - y[j];
            ans = min(ans, d_x * d_x + d_y * d_y);
        }
    }
    cout << fixed << setprecision(4) << sqrt(ans) << endl;
    return 0;
}
