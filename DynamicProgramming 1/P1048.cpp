/**
 * Luogu P1048 - [NOIP 2005 普及组] 采药
 *
 * 问题描述：
 *   有 m 株草药，每株草药至多采摘一次。第 i 株草药需要一定时间并具有相应价值，
 *   在总时间不超过 t 的条件下，求能获得的最大总价值。
 *
 * 解法：0/1 背包动态规划
 *   设 dp[i][j] 表示只考虑前 i 株草药、可用时间不超过 j 时能够获得的最大价值。
 *   对于第 i 株草药，若 j 小于其采摘时间 it，只能不采摘；否则比较不采摘与采摘
 *   当前草药两种选择：
 *       dp[i][j] = max(dp[i-1][j], dp[i-1][j-it]+val)
 *   第二项使用上一行 dp[i-1]，保证每株草药只会被选择至多一次。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(mt)
 *   - 空间复杂度：O(mt)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int t, m;
    cin >> t >> m;
    // v[i] 依次保存第 i 株草药的采摘时间和价值。
    vector<pair<int, int>> v(m);
    for (int i = 0; i < m; i++) {
        cin >> v[i].first >> v[i].second;
    }

    // dp[i][j] 表示只考虑前 i 株草药、时间不超过 j 时的最大价值。
    vector dp(m + 1, vector(t + 1, 0));
    for (int i = 1; i <= m; ++i) {
        // 当前处理第 i 株草药，其下标为 i-1。
        auto [it, val] = v[i - 1];
        for (int j = 1; j <= t; ++j) {
            if (j >= it) {
                // 比较不采当前草药与采一株当前草药后的总价值。
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - it] + val);
            } else {
                // 可用时间不足，只能继承前 i-1 株草药的最优结果。
                dp[i][j] = dp[i - 1][j];
            }
        }
    }
    // dp[m][t] 即所有草药都考虑完后，在总时间 t 内的最大价值。
    cout << dp[m][t] << endl;
    return 0;
}
