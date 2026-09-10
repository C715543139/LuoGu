/**
 * Luogu P1725 - 琪露诺
 *
 * 问题描述：
 *   从编号 0 的冰格出发，每次向右跳 L 到 R 格，落到某个冰格时获得该格的分数。
 *   当下一次跳跃能够越过编号 n 的冰格时游戏结束，求能够获得的最大分数。
 *
 * 解法：动态规划 + 单调队列优化
 *   设 dp[i] 表示跳到第 i 个冰格时能够获得的最大分数。
 *   要到达位置 i，上一个位置 j 必须满足：
 *     i - R <= j <= i - L
 *   因此状态转移为：
 *     dp[i] = max(dp[j]) + val[i]，其中 j 属于 [i-R, i-L]。
 *
 *   随着 i 向右移动，合法前驱区间也整体向右移动，可以使用单调队列维护区间内
 *   dp 的最大值。队首始终保存当前区间内 dp 值最大的状态：
 *   - 将新进入区间的下标 i-L 加入队尾，并删除队尾所有 dp 值更小的状态；
 *   - 删除队首已经小于 i-R、离开合法区间的状态；
 *   - 使用队首状态更新 dp[i]。
 *
 *   游戏结束前最后一次落脚的位置必须位于 [n-R+1, n]，从这些状态中取最大值。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n)，每个状态至多入队、出队各一次；
 *   - 空间复杂度：O(n)。
 */

#include <iostream>
#include <vector>
#include <climits>
#include <deque>
using namespace std;

int main() {
    // 读入终点编号以及每次跳跃距离的下界和上界
    int n, l, r;
    cin >> n >> l >> r;

    // val[i] 表示第 i 个冰格的分数，dp[i] 表示到达该冰格时的最大得分
    vector<long long> val(n + 1, 0), dp(n + 1, INT_MIN);
    for (int i = 0; i < n + 1; i++) {
        cin >> val[i];
    }

    // 单调队列存储 {dp 值, 下标}，队首为当前合法前驱中的最大 dp 值
    deque<pair<long long, int>> dq;

    // 从编号 0 的冰格出发，此时尚未获得分数
    dp[0] = 0;
    for (int i = l; i <= n; i++) {
        // 能够一步跳到 i 的前驱下标范围为 [i-r, i-l]
        int start = i - r, end = i - l;

        // end 是本轮新进入合法区间的下标，将其加入单调队列
        if (dq.empty() || dp[end] <= dq.back().first) {
            dq.emplace_back(dp[end], end);
        } else {
            // 删除所有 dp 值小于新状态的队尾元素，维持 dp 值单调不增
            while (!dq.empty() && dq.back().first < dp[end]) {
                dq.pop_back();
            }
            dq.emplace_back(dp[end], end);
        }

        // 删除已经离开合法前驱区间左端的状态
        while (dq.front().second < start) {
            dq.pop_front();
        }

        // 队首是当前合法区间内得分最大的前驱
        dp[i] = dq.front().first + val[i];
    }

    // 最后落在 [n-r+1, n] 时，下一跳可以越过 n 并结束游戏
    long long res = INT_MIN;
    for (int i = n - r + 1; i <= n; i++) {
        res = max(res, dp[i]);
    }

    // 输出游戏过程中能够获得的最大分数
    cout << res << endl;
    return 0;
}
