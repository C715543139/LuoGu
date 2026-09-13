/**
 * Luogu P3183 - 食物链
 *
 * 问题描述：
 *   在一张表示捕食关系的有向无环图中，统计从生产者到最高级消费者的食物链条数。
 *   没有任何捕食关系的孤立生物不计入答案。
 *
 * 解法：记忆化 DFS
 *   入度为 0 的节点是生产者。设 dp[u] 表示从 u 出发到达任意出度为 0 节点的
 *   路径条数：若 u 没有后继，则 dp[u]=1；否则 dp[u] 等于所有后继 dp 值之和。
 *   从每个生产者开始搜索，并将非孤立生产者的 dp 值累加为答案。
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
vector<int> in, dp;

void dfs(int node) {
    // 出度为 0 的节点是食物链终点，从自身到终点恰有一条路径
    if (graph[node].empty()) {
        dp[node] = 1;
        return;
    }

    // 已经计算过该节点，无需重复搜索其后继
    int &res = dp[node];
    if (res != 0) {
        return;
    }

    // 从 node 出发的路径数等于从各个直接后继出发的路径数之和
    for (int v: graph[node]) {
        dfs(v);
        res += dp[v];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    graph.assign(n + 1, {});
    in.assign(n + 1, 0);
    for (int i = 1; i <= m; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        in[b]++;
    }

    dp.assign(n + 1, 0);
    // 只从入度为 0 的生产者开始统计完整食物链
    for (int i = 1; i <= n; i++) {
        if (in[i] == 0) {
            dfs(i);
        }
    }

    int sum = 0;
    for (int i = 1; i <= n; i++) {
        // 排除既无入边也无出边的孤立节点
        if (in[i] == 0 && !graph[i].empty()) {
            sum += dp[i];
        }
    }
    cout << sum << endl;
    return 0;
}
