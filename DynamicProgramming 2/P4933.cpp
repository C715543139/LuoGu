/**
 * Luogu P4933 - 大师
 *
 * 问题描述：
 *   从给定的电塔序列中选出一个非空子序列，使选出的高度从左到右构成等差数列，
 *   求所有满足条件的选择方案数。长度为 1 或 2 的序列也属于等差数列。
 *
 * 解法：动态规划
 *   设 dp[i][d] 表示以第 i 个电塔结尾、公差为 d 的等差子序列数量，其中序列长度至少为 2。
 *
 *   枚举当前电塔 i 和它的前一个电塔 j，令 d = h[i] - h[j]：
 *   - 以 j 结尾且公差为 d 的等差子序列，都可以接上 i；
 *   - 选取 j 和 i 本身，可以新得到一个长度为 2 的等差子序列。
 *
 *   因此，本次从 j 转移到 i 新增加的方案数为 dp[j][d] + 1，
 *   并将其累加到 dp[i][d] 和最终答案中。
 *   由于长度为 1 的序列一定合法，答案初始化为 n。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n^2)
 *   - 空间复杂度：O(n^2)，使用哈希表保存每个位置实际出现过的公差状态。
 */

#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

const int MOD = 998244353;

int main() {
    // 优化输入输出效率
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读入电塔数量和每个电塔的高度
    int n;
    cin >> n;
    vector<int> h(n);
    for (int &height : h) {
        cin >> height;
    }

    // 长度为 1 的子序列均为合法方案
    int res = n;

    // dp[i][d] 表示以第 i 个位置结尾、公差为 d 的等差子序列数量
    vector dp(n, unordered_map<int, int>{});

    // 枚举等差子序列的最后两个位置 j 和 i
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            // 以 j、i 作为末尾两个元素时，等差数列的公差为 d
            int d = h[i] - h[j], newWays = dp[j][d] + 1;

            // dp[j][d] 可以接上 i，j 和 i 本身也构成一个新的二元序列
            dp[i][d] = (newWays % MOD + dp[i][d] % MOD) % MOD;

            // 只将本次转移新增的方案计入答案，避免重复统计
            res = (res % MOD + newWays % MOD) % MOD;
        }
    }
    cout << res << endl;
    return 0;
}
