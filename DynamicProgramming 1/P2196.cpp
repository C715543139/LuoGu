/**
 * Luogu P2196 - [NOIP 1996 提高组] 挖地雷
 *
 * 问题描述：
 *   每个地窖有一定数量的地雷，只能从编号较小的地窖移动到与其连通的编号较大地窖。
 *   可以任选起点，求一条能够挖到最多地雷的路径，并输出路径及地雷总数。
 *
 * 解法：有向无环图上的动态规划
 *   将每条连接视为从较小编号指向较大编号的有向边，图中不会出现环。设 dp[i] 表示
 *   从 i 号地窖开始、按规则走到终点时能挖到的最多地雷数，则：
 *       dp[i] = w[i] + max(dp[to])，其中 i 可以到达 to
 *   按编号从大到小计算 dp，转移时所有后继地窖的 dp 值都已经确定。next[i] 记录使
 *   dp[i] 取得最大值的下一处地窖，用于最后还原路径。
 *
 * 复杂度分析：
 *   需要读取并枚举上三角中的所有连接关系：
 *   - 时间复杂度：O(n^2)
 *   - 空间复杂度：O(n^2)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    // w[i] 是编号为 i+1 的地窖中埋藏的地雷数量。
    vector<int> w(n);
    for (int i = 0; i < n; i++) {
        cin >> w[i];
    }
    // graph[i][j] 表示 i 号地窖能否走到编号为 i+j+1 的地窖。
    vector<vector<int>> graph(n);
    for (int i = 0; i < n; i++) {
        graph[i].assign(n - i - 1, 0);
        for (int j = 0; j < n - i - 1; j++) {
            cin >> graph[i][j];
        }
    }

    // dp[i] 是从 i 号地窖出发能挖到的最多地雷数；next[i] 用于还原最优路径。
    int res = 0;
    vector dp(n, 0), next(n, -1);
    for (int i = 0; i < n; i++) {
        // 不继续移动时，路径只包含当前地窖。
        dp[i] = w[i];
        res = max(res, dp[i]);
    }

    // 后继地窖的编号都更大，因此从后向前计算可以直接使用它们的 dp 值。
    for (int i = n - 2; i >= 0; i--) {
        for (int j = 0; j < n - i - 1; j++) {
            // graph[i][j] 对应的后继节点下标为 i+j+1。
            if (graph[i][j] && dp[i] < w[i] + dp[i + j + 1]) {
                // 经该后继节点继续前进，可以得到更大的路径和，记录路径选择。
                next[i] = i + j + 1;
                dp[i] = w[i] + dp[i + j + 1];
                res = max(res, dp[i]);
            }
        }
    }

    // 任意地窖都可作为起点，找到总地雷数等于 res 的起点后沿 next 输出路径。
    for (int i = 0; i < n; i++) {
        if (dp[i] == res) {
            cout << i + 1 << ' ';
            for (int j = next[i]; j != -1; j = next[j]) {
                // 每次跳到记录的最优后继地窖，直到到达没有后继的终点。
                cout << j + 1 << ' ';
            }
            cout << endl;
            break;
        }
    }
    cout << res << endl;
    return 0;
}
