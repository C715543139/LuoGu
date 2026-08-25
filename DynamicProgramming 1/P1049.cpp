/**
 * Luogu P1049 - [NOIP 2001 普及组] 装箱问题
 *
 * 问题描述：
 *   有一个容量为 V 的箱子和 n 个物品，每个物品有各自的体积。可任取若干物品装入
 *   （也可一个都不取），求箱子剩余空间的最小值。
 *
 * 解法：0/1 背包变种的动态规划
 *   设 dp[i][j] 表示只考虑前 i 个物品、容量上限为 j 时的最小剩余空间（以总容量 V 为
 *   基准，等价于 V 减去能装入的最大体积）。第 i 个物品体积为 vi：
 *     - 若 j < vi，放不下该物品，只能继承：dp[i][j] = dp[i-1][j]
 *     - 若 j >= vi，比较“不放入”与“放入”两种情况：
 *         不放入：dp[i-1][j]
 *         放入：先只用容量 j-vi 装前 i-1 个物品，剩余 dp[i-1][j-vi]，再腾出 vi 给
 *               当前物品，故为 dp[i-1][j-vi] - vi
 *       取两者较小值。
 *   初值 dp[0][j] = V（不装任何物品时装入体积为 0，剩余空间为 V）。答案即 dp[n][V]。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(nV)
 *   - 空间复杂度：O(nV)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int v, n;
    cin >> v >> n;
    // vt[i] 保存第 i 个物品的体积。
    vector vt(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        cin >> vt[i];
    }

    // dp[i][j] 表示只考虑前 i 个物品、容量上限为 j 时的最小剩余空间，初值全部为 V。
    vector dp(n + 1, vector(v + 1, v));
    for (int i = 1; i <= n; ++i) {
        int vi = vt[i];
        for (int j = 0; j <= v; ++j) {
            if (j >= vi) {
                // 比较不放入第 i 个物品与放入后剩余空间两种情况，取较小值。
                dp[i][j] = min(dp[i - 1][j], dp[i - 1][j - vi] - vi);
            } else {
                // 容量不足，放不下，只能继承前 i-1 个物品的结果。
                dp[i][j] = dp[i - 1][j];
            }
        }
    }
    // dp[n][v] 即容量 V 下考虑所有物品后的最小剩余空间。
    cout << dp[n][v] << endl;
    return 0;
}
