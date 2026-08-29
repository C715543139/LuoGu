/**
 * Luogu P1854 - [IOI 1999] 花店橱窗布置
 *
 * 问题描述：
 *   将 F 束花按照编号顺序放入 V 个花瓶中，每个花瓶至多放一束花，
 *   并且每束花都必须放入花瓶。求最大的美学值，同时输出一种最优摆放方案。
 *
 * 解法：动态规划
 *   设 dp[i][j] 表示将前 i 束花放入前 j 个花瓶时能够获得的最大美学值。
 *   因为花束必须按照编号顺序摆放，所以第 i 束花如果放入第 j 个花瓶，
 *   前 i-1 束花只能放入前 j-1 个花瓶。
 *
 *   对于第 j 个花瓶，有以下两种选择：
 *   1. 第 j 个花瓶空着：dp[i][j] = dp[i][j-1]；
 *   2. 第 i 束花放入第 j 个花瓶：
 *      dp[i][j] = dp[i-1][j-1] + vals[i-1][j-1]。
 *
 *   因此状态转移为：
 *     dp[i][j] = max(dp[i][j-1], dp[i-1][j-1] + vals[i-1][j-1])。
 *
 *   pre[i][j] 记录 dp[i][j] 的转移来源：
 *   - pre[i][j] = j-1：第 j 个花瓶为空；
 *   - pre[i][j] = j：第 i 束花放入第 j 个花瓶。
 *   根据 pre 数组从 dp[f][v] 反向回溯，即可恢复一种最优方案。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(FV)
 *   - 空间复杂度：O(FV)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 读入花束数量和花瓶数量
    int f, v;
    cin >> f >> v;

    // vals[i][j] 表示第 i+1 束花放入第 j+1 个花瓶时的美学值
    vector vals(f, vector(v, 0));
    for (int i = 0; i < f; i++) {
        for (int j = 0; j < v; j++) {
            cin >> vals[i][j];
        }
    }

    // 使用一个足够小的数表示不可能的状态
    const int NEG_INF = -1e9;

    // dp[i][j] 表示前 i 束花放入前 j 个花瓶时的最大美学值
    vector dp(f + 1, vector(v + 1, NEG_INF));

    // pre[i][j] 用于记录 dp[i][j] 的转移来源，便于恢复摆放方案
    vector pre(f + 1, vector(v + 1, 0));

    // 没有花束时，美学值为 0，与使用多少个花瓶无关
    for (int j = 0; j <= v; ++j) {
        dp[0][j] = 0;
    }

    // 枚举已经放置的花束数量和已经考虑的花瓶数量
    for (int i = 1; i <= f; ++i) {
        for (int j = 1; j <= v; ++j) {
            // 第 j 个花瓶空着
            dp[i][j] = dp[i][j - 1];
            pre[i][j] = j - 1;

            // 第 i 束花放入第 j 个花瓶
            int put = dp[i - 1][j - 1] + vals[i - 1][j - 1];
            if (put > dp[i][j]) {
                dp[i][j] = put;
                pre[i][j] = j;
            }
        }
    }

    // 从最终状态开始反向回溯，恢复每束花所在的花瓶编号
    vector<int> answer(f + 1);
    int i = f, j = v;
    while (i > 0) {
        if (pre[i][j] == j) {
            // 第 i 束花放入了第 j 个花瓶
            answer[i] = j;
            --i;
            --j;
        } else {
            // 第 j 个花瓶为空，继续检查前一个花瓶
            j = pre[i][j];
        }
    }

    // 输出最大美学值
    cout << dp[f][v] << '\n';

    // 输出每束花对应的花瓶编号
    for (int flower = 1; flower <= f; ++flower) {
        cout << answer[flower] << (flower == f ? '\n' : ' ');
    }

    return 0;
}
