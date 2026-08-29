/**
 * Luogu P1775 - 石子合并（弱化版）
 *
 * 问题描述：
 *   有若干堆石子排成一排，每次只能合并相邻的两堆石子，
 *   合并代价为这两堆石子的总质量，求将所有石子合并成一堆的最小总代价。
 *
 * 解法：区间动态规划
 *   由于每次只能合并相邻石子，任意中间状态都对应原序列中的一个连续区间。
 *   设 dp[l][r] 表示将第 l 堆到第 r 堆石子合并成一堆的最小代价。
 *
 *   考虑区间 [l, r] 的最后一次合并。设最后一次合并的分界点为 k，
 *   那么最后合并的两堆分别由区间 [l, k] 和 [k+1, r] 合并而来。
 *   因此需要先求出两个子区间的最小合并代价，最后再支付整个区间的石子总质量：
 *     dp[l][r] = min(dp[l][k] + dp[k+1][r] + sum(l, r))。
 *
 *   使用前缀和数组 prefix，可以在 O(1) 时间内计算区间 [l, r] 的石子总质量：
 *     sum(l, r) = prefix[r] - prefix[l-1]。
 *
 *   只有一堆石子时不需要进行合并，因此 dp[i][i] = 0，
 *   代码通过初始化二维数组为 0 来表示该边界状态。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n^3)，枚举区间长度、左端点和分界点；
 *   - 空间复杂度：O(n^2)。
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 读入石子堆数
    int n;
    cin >> n;

    // stones[i] 表示第 i 堆石子的质量，数组下标从 1 开始
    vector<int> stones(n + 1);

    // prefix[i] 表示前 i 堆石子的质量总和
    vector prefix(n + 1, 0);

    // 读入每堆石子的质量，并计算前缀和
    for (int i = 1; i <= n; ++i) {
        cin >> stones[i];
        prefix[i] = prefix[i - 1] + stones[i];
    }

    // dp[l][r] 表示将第 l 堆到第 r 堆合并成一堆的最小代价
    vector dp(n + 1, vector(n + 1, 0));

    // 按区间长度从小到大计算，保证转移所需的子区间已经求出
    for (int length = 2; length <= n; ++length) {
        for (int l = 1; l + length - 1 <= n; ++l) {
            int r = l + length - 1;

            // 当前区间至少需要进行一次合并，先初始化为足够大的数
            dp[l][r] = 1e9;

            // 枚举最后一次合并的分界点 k
            for (int k = l; k < r; ++k) {
                // 左右子区间分别合并完成后，再合并整个区间
                int left = dp[l][k];
                int right = dp[k + 1][r];

                // 最后一次合并的代价等于整个区间内石子的总质量
                int total = prefix[r] - prefix[l - 1];

                // 在所有分界点中选择总代价最小的方案
                dp[l][r] = min(dp[l][r], left + right + total);
            }
        }
    }

    // 输出将全部石子合并成一堆的最小总代价
    cout << dp[1][n] << endl;
    return 0;
}
