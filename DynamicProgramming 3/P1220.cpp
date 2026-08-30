/**
 * Luogu P1220 - 关路灯
 *
 * 问题描述：
 *   有 n 盏路灯沿一条直线排列，老张从第 c 盏路灯开始关灯。
 *   关掉一盏路灯所需的时间可以忽略，路灯关闭前会持续消耗电量，
 *   求关掉所有路灯时的最小总耗电量。
 *
 * 解法：区间动态规划
 *   老张每次只能去关闭当前已关闭区间左侧或右侧紧邻的路灯，
 *   因此已经关闭的路灯始终构成一个包含第 c 盏路灯的连续区间 [l, r]。
 *
 *   设 dp[l][r][0] 表示已经关闭区间 [l, r]，且老张当前位于第 l 盏路灯处时的最小耗电量；
 *   设 dp[l][r][1] 表示已经关闭区间 [l, r]，且老张当前位于第 r 盏路灯处时的最小耗电量。
 *
 *   设当前仍亮着的路灯总功率为 remain：
 *     remain = 所有路灯总功率 - 区间 [l, r] 内路灯的总功率。
 *   老张移动一段距离时，这些仍亮着的路灯会持续耗电，
 *   因为速度为 1 m/s，所以移动距离也就是移动时间，产生的耗电量为：
 *     移动距离 * remain。
 *
 *   如果去关闭左侧第 l-1 盏路灯，区间变为 [l-1, r]，并且老张位于新区间左端；
 *   如果去关闭右侧第 r+1 盏路灯，区间变为 [l, r+1]，并且老张位于新区间右端。
 *   按区间长度从小到大进行转移即可。
 *
 *   使用前缀和 prefix，可以在 O(1) 时间内计算区间 [l, r] 内路灯的总功率：
 *     sum(l, r) = prefix[r] - prefix[l-1]。
 *
 *   初始时老张立即关闭第 c 盏路灯，因此：
 *     dp[c][c][0] = dp[c][c][1] = 0。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n^2)，每个区间状态只进行常数次转移；
 *   - 空间复杂度：O(n^2)。
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

int main() {
    // 读入路灯数量和老张开始关灯的位置
    int n, c;
    cin >> n >> c;

    // position[i] 表示第 i 盏路灯的位置，power[i] 表示其功率
    // 路灯编号从 1 开始，位置已经按照从小到大的顺序给出
    vector<long long> position(n + 1), power(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> position[i] >> power[i];
    }

    // prefix[i] 表示前 i 盏路灯的总功率
    vector<long long> prefix(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        prefix[i] = prefix[i - 1] + power[i];
    }

    // dp[l][r][0/1] 分别表示当前位于区间左端/右端时的最小耗电量
    constexpr long long inf = numeric_limits<long long>::max() / 4;
    vector dp(n + 2, vector(n + 2, vector(2, inf)));

    // 老张一开始就在第 c 盏路灯旁，并立即将其关闭
    dp[c][c][0] = 0;
    dp[c][c][1] = 0;

    // 按已经关闭的区间长度从小到大进行状态转移
    for (int len = 1; len <= n; ++len) {
        for (int l = 1; l + len - 1 <= n; ++l) {
            int r = l + len - 1;

            // 已关闭区间必须包含第 c 盏路灯，否则该状态无法从初始状态到达
            if (c < l || c > r) {
                continue;
            }

            // 当前区间之外的路灯仍然亮着，它们决定接下来移动时的耗电速度
            long long remain = prefix[n] - (prefix[r] - prefix[l - 1]);

            // 去关闭左侧相邻的第 l-1 盏路灯
            if (l > 1) {
                // 当前位于右端 r，需要走到 l-1
                long long distanceFromRight = position[r] - position[l - 1];
                dp[l - 1][r][0] = min(
                    dp[l - 1][r][0],
                    dp[l][r][1] + distanceFromRight * remain
                );

                // 当前位于左端 l，需要走到 l-1
                long long distanceFromLeft = position[l] - position[l - 1];
                dp[l - 1][r][0] = min(
                    dp[l - 1][r][0],
                    dp[l][r][0] + distanceFromLeft * remain
                );
            }

            // 去关闭右侧相邻的第 r+1 盏路灯
            if (r < n) {
                // 当前位于左端 l，需要走到 r+1
                long long distanceFromLeft = position[r + 1] - position[l];
                dp[l][r + 1][1] = min(
                    dp[l][r + 1][1],
                    dp[l][r][0] + distanceFromLeft * remain
                );

                // 当前位于右端 r，需要走到 r+1
                long long distanceFromRight = position[r + 1] - position[r];
                dp[l][r + 1][1] = min(
                    dp[l][r + 1][1],
                    dp[l][r][1] + distanceFromRight * remain
                );
            }
        }
    }

    // 所有路灯都已关闭，老张最后可能位于第 1 盏或第 n 盏路灯处
    cout << min(dp[1][n][0], dp[1][n][1]) << endl;
    return 0;
}
