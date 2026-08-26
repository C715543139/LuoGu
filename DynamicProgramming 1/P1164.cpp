/**
 * Luogu P1164 - 小 A 点菜
 *
 * 问题描述：
 *   有 N 种菜，每种菜只有一份，第 i 种菜价格为 a_i。小 A 口袋里有 M 元，要求点菜
 *   恰好花光 M 元，求点菜方案数。
 *
 * 解法：0/1 背包计数型动态规划
 *   设 dp[i][j] 表示只考虑前 i 种菜、恰好花 j 元时的点菜方案数。第 i 种菜价格为 val：
 *     - 若 j < val，买不起该菜，只能不点：dp[i][j] = dp[i-1][j]
 *     - 若 j >= val，可点可不点，两者方案相加：
 *         dp[i][j] = dp[i-1][j] + dp[i-1][j-val]
 *   初值 dp[0][0] = 1（什么都不点、花 0 元这一种方案），其余为 0。答案即 dp[N][M]。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(NM)
 *   - 空间复杂度：O(NM)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    // v[i] 保存第 i 种菜的价格。
    vector<int> v(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> v[i];
    }

    // dp[i][j] 表示只考虑前 i 种菜、恰好花 j 元时的方案数。
    vector dp(n + 1, vector(m + 1, 0));
    // 什么都不点、花 0 元是一种方案，作为递推起点。
    dp[0][0] = 1;
    for (int i = 1; i <= n; ++i) {
        int val = v[i];
        for (int j = 0; j <= m; ++j) {
            if (j >= val) {
                // 钱够，可以“不点”或“点”该菜，两种情况的方案数相加。
                dp[i][j] = dp[i - 1][j] + dp[i - 1][j - val];
            } else {
                // 钱不够，只能不点，直接继承前 i-1 种菜的结果。
                dp[i][j] = dp[i - 1][j];
            }
        }
    }
    // dp[n][m] 即考虑全部菜、恰好花光 M 元的方案数。
    cout << dp[n][m] << endl;
    return 0;
}
