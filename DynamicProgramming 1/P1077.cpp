/**
 * Luogu P1077 - [NOIP 2012 普及组] 摆花
 *
 * 问题描述：
 *   花店门口要摆一排共 m 盆花，共有 n 种花，第 i 种花不能超过 a_i 盆。同一种花必须
 *   放在一起，且不同种类的花按标号从小到大依次排列，求不同的摆花方案数。
 *
 * 解法：有界背包计数型动态规划
 *   由于同种花必须相邻、种类顺序固定，一种方案完全由“每种花各摆几盆”决定。设
 *   dp[i][j] 表示只考虑前 i 种花、共摆 j 盆时的方案数。第 i 种花可摆 k 盆（0 <= k <= a_i），
 *   因此：
 *       dp[i][j] = sum_{k=0}^{a_i} dp[i-1][j-k]   （其中 j >= k）
 *   初值 dp[0][0] = 1（什么都不摆这一种方案）。答案即 dp[n][m]，过程中对 10^6+7 取模。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n * m * max(a_i))
 *   - 空间复杂度：O(nm)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    // v[i] 保存第 i 种花最多能摆的盆数。
    vector<int> v(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> v[i];
    }

    // dp[i][j] 表示只考虑前 i 种花、共摆 j 盆时的方案数。
    vector dp(n + 1, vector(m + 1, 0));
    // 什么都不摆是唯一一种用 0 盆花构成前 0 种花的方案。
    dp[0][0] = 1;
    for (int i = 1; i <= n; ++i) {
        int val = v[i];
        // 枚举第 i 种花摆 k 盆（k 不能超过该种花的上限 val）。
        for (int k = 0; k <= val; ++k) {
            for (int j = 0; j <= m; ++j) {
                if (j >= k) {
                    // 前 i-1 种花摆 j-k 盆，加上当前种的 k 盆，累加方案数并对 10^6+7 取模。
                    dp[i][j] = (dp[i - 1][j - k] % 1000007 + dp[i][j] % 1000007) % 1000007;
                }
            }
        }
    }
    // dp[n][m] 即考虑全部 n 种花、共摆 m 盆的方案数。
    cout << dp[n][m] << endl;
    return 0;
}
