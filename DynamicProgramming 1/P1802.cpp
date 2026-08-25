/**
 * Luogu P1802 - 5 倍经验日
 *
 * 问题描述：
 *   absi2011 有 x 个迷你装药物，要依次挑战 n 个好友。每个好友需要至少 use 个药才能
 *   打赢；若用药不足，则必然失败且消耗的药物浪费。失败可得 lose 经验，胜利可得 win
 *   经验。求能获得的最大经验 s 并输出 5s。
 *
 * 解法：0/1 背包变种的动态规划
 *   设 dp[i][j] 表示只考虑前 i 个好友、恰好消耗 j 个药时能获得的最大经验。对第 i 个
 *   好友有两种选择：失败不消耗药物，直接得到 lose[i]；胜利则消耗 use[i] 个药物得到
 *   win[i]。因此当 j >= use[i] 时比较两种选择，否则只能失败：
 *       dp[i][j] = max(dp[i-1][j] + lose[i], dp[i-1][j-use[i]] + win[i])  （j >= use[i]）
 *       dp[i][j] = dp[i-1][j] + lose[i]                                   （j <  use[i]）
 *   最终答案为 dp[n][x] * 5。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(nx)
 *   - 空间复杂度：O(nx)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, x;
    cin >> n >> x;

    // lose[i]/win[i]/use[i] 分别保存第 i 个好友失败经验、胜利经验和最少所需药量。
    vector lose(n + 1, 0), win(n + 1, 0), use(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        cin >> lose[i] >> win[i] >> use[i];
    }

    // dp[i][j] 表示只考虑前 i 个好友、恰好消耗 j 个药时能获得的最大经验。
    vector dp(n + 1, vector(x + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j <= x; ++j) {
            if (j >= use[i]) {
                // 药物足够时，比较“失败拿 lose[i]”与“消耗 use[i] 打赢拿 win[i]”。
                dp[i][j] = max(dp[i - 1][j] + lose[i], dp[i - 1][j - use[i]] + win[i]);
            } else {
                // 药物不足，只能失败拿到 lose[i]。
                dp[i][j] = dp[i - 1][j] + lose[i];
            }
        }
    }
    // 输出 5 倍经验，用 5ll 避免中途溢出。
    cout << dp[n][x] * 5ll << endl;
    return 0;
}
