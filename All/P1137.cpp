/**
 * Luogu P1137 - 旅行计划
 *
 * 问题描述：
 *   给定一张有向无环图，对每个节点求一条以该节点为终点的最长路径所包含的
 *   节点数量。
 *
 * 解法：拓扑序上的动态规划
 *   dp[u] 表示以 u 为终点的最长路径长度。所有入度为 0 的节点令 dp=1。
 *   处理节点 u 的每条出边 u-v 时，用 dp[u]+1 更新 dp[v]，同时削减 v 的剩余
 *   入度；当 v 的所有前驱均已处理后，继续递归处理 v。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n+m)
 *   - 空间复杂度：O(n+m)
 */

#include <iostream>
#include <vector>
using namespace std;

int n, m;
vector<vector<int>> graph;
vector<int> dp, in;

void dfs(int node) {
    for (int v: graph[node]) {
        // 用经过 node 的路径更新以 v 为终点的最长路径
        dp[v] = max(dp[v], dp[node] + 1);
        // 删除已经处理完毕的前驱边；入度归零后即可处理 v
        in[v]--;
        if (in[v] == 0) {
            dfs(v);
            // 将已经递归处理的节点标记为负数，避免主循环再次将其作为起点
            in[v]--;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    graph.assign(n + 1, {});
    in.assign(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        in[v]++;
    }

    dp.assign(n + 1, 0);
    // 每个源点自身构成长度为 1 的初始路径
    for (int i = 1; i <= n; i++) {
        if (in[i] == 0) {
            dp[i] = 1;
            dfs(i);
        }
    }

    for (int i = 1; i <= n; i++) {
        cout << dp[i] << endl;
    }
    return 0;
}
