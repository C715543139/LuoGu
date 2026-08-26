/**
 * Luogu P3842 - [TJOI2007] 线段
 *
 * 问题描述：
 *   在 n×n 平面上，第 i 行有一条线段 [L_i, R_i]。从 (1,1) 出发，只能向下、向左或
 *   向右移动，且向下之前必须走完本行的整条线段，最终到达 (n,n)，求最短路程。
 *
 * 解法：动态规划
 *   由于不能向上走，只能按行从上到下依次处理。走完整条线段后，最优落点必在线段
 *   端点 L_i 或 R_i 上（停在中间只是折返多走，而左右移动在任意一行代价相同，可推迟
 *   到下一行再做）。设 f[i][0]/f[i][1] 表示走完前 i 行、最后停在 L_i/R_i 的最短距离。
 *
 *   从第 i-1 行的端点 p（L_{i-1} 或 R_{i-1}）下降到第 i 行同一列，再走完整条线段：
 *     停在 L_i：水平代价 = (R_i-L_i) + |R_i - p|   （先到右端再折回左端）
 *     停在 R_i：水平代价 = (R_i-L_i) + |L_i - p|   （先到左端再到右端）
 *   再加上下降的 1 步，得到：
 *     f[i][0] = (R_i-L_i) + min(f[i-1][0]+|R_i-L_{i-1}|, f[i-1][1]+|R_i-R_{i-1}|) + 1
 *     f[i][1] = (R_i-L_i) + min(f[i-1][0]+|L_i-L_{i-1}|, f[i-1][1]+|L_i-R_{i-1}|) + 1
 *
 *   初值（第 1 行从 (1,1) 出发，无需下降）：
 *     f[1][0] = (R_1-L_1) + (R_1-1)
 *     f[1][1] = (R_1-L_1) + (L_1-1)
 *   答案（最后从第 n 行端点向右走到 (n,n)）：
 *     ans = min(f[n][0] + (n-L_n), f[n][1] + (n-R_n))
 *
 *   转移只依赖上一行，故用两个变量滚动即可，无需保存整张表。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n)
 *   - 空间复杂度：O(1)
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

int main() {
    int n;
    cin >> n;
    // lr[i] 保存第 i 行线段的左右端点 (L_i, R_i)。
    vector<pair<int, int>> lr(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> lr[i].first >> lr[i].second;
    }

    // pl/pr 为上一行线段的左右端点列；prevL/prevR 为走完上一行后停在左/右端点的最短距离。
    long long pl = lr[1].first, pr = lr[1].second;
    // 第 1 行从 (1,1) 出发：先走到右端再折回左端，或直接走到右端。
    long long prevL = (pr - pl) + (pr - 1);
    long long prevR = (pr - pl) + (pl - 1);

    for (int i = 2; i <= n; ++i) {
        long long l = lr[i].first, r = lr[i].second;
        long long len = r - l;
        // 从上一行端点下降 1 步，再走完整条线段：
        //   curL 停在 L_i（先到右端 r 再折回 l），curR 停在 R_i（先到左端 l 再到 r）。
        long long curL = len + min(prevL + abs(r - pl), prevR + abs(r - pr)) + 1;
        long long curR = len + min(prevL + abs(l - pl), prevR + abs(l - pr)) + 1;
        // 滚动更新为当前行的两个状态。
        prevL = curL;
        prevR = curR;
        pl = l;
        pr = r;
    }

    // 最后从第 n 行端点向右走到 (n,n)，取两种停法的较小值。
    cout << min(prevL + (n - pl), prevR + (n - pr)) << endl;
    return 0;
}
