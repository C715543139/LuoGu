/**
 * Luogu P1616 - 疯狂的采药
 *
 * 问题描述：
 *   共有 m 种草药，每种草药采一次需要时间 a 并具有价值 b，且同一种草药可以无限次
 *   采摘。在总时间不超过 t 的条件下，求能获得的最大总价值。
 *
 * 解法：完全背包（无界背包）动态规划
 *   设 dp[i] 表示用 i 单位时间能采到的最大总价值。由于每种草药可无限采摘，枚举到
 *   容量 i 时，dp[i - it] 已经包含了再次采摘同一种草药的可能，因此递推：
 *       dp[i] = max(dp[i], dp[i - it] + val)   （对所有 it <= i 的草药）
 *   这里外层枚举时间、内层枚举草药，等价于标准完全背包的“先枚举物品、再正序枚举容量”，
 *   二者都能保证同一种草药被重复利用。答案即 dp[t]。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(mt)
 *   - 空间复杂度：O(t)（一维滚动数组）
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int t, m;
    cin >> t >> m;
    // items[i] 保存第 i 种草药的采摘时间和价值。
    vector<pair<int, int>> items(m);
    for (int i = 0; i < m; ++i) {
        cin >> items[i].first >> items[i].second;
    }

    // dp[i] 表示用 i 单位时间能采到的最大总价值，价值可能较大，用 unsigned long long 保存。
    vector<unsigned long long> dp(t + 1, 0);
    for (int i = 1; i <= t; ++i) {
        // 枚举每种草药，尝试在时间为 i 时采摘它（可重复采摘）。
        for (auto [it, val] : items) {
            if (it <= i) {
                // 摘一株该草药后，剩余时间 i-it 的最优解已算好，加上本株价值取最大。
                dp[i] = max(dp[i], dp[i - it] + val);
            }
        }
    }
    // dp[t] 即总时间为 t 时的最大总价值。
    cout << dp[t] << endl;
    return 0;
}
