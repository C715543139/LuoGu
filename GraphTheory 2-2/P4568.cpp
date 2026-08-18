/**
 * Luogu P4568 - [JLOI2011] 飞行路线
 *
 * 问题描述：
 *   给定一个包含 n 个城市、m 条双向航线的非负权图，从城市 s 前往城市 t。途中最多
 *   可以选择 k 条航线免费乘坐，求能够得到的最小总费用。
 *
 * 解法：分层状态 + 堆优化 Dijkstra
 *   只记录所在城市无法表示还剩多少次免费机会，因此增加“已经使用的免费次数”这一维：
 *       dist[u][j]
 *   表示从 s 到达城市 u，并且恰好使用了 j 次免费机会时的最小费用。每个状态
 *   (u,j) 可以看成分层图中的一个节点，原图的每个城市在第 0..k 层各有一份。
 *
 *   对于原图中的航线 u-v，价格为 w，从状态 (u,j) 出发有两种转移：
 *   1. 正常付费：到达 (v,j)，新费用为 dist[u][j]+w；
 *   2. 免费乘坐：当 j<k 时到达 (v,j+1)，新费用仍为 dist[u][j]。
 *   所有转移的边权都是非负数，可以在这些分层状态上运行 Dijkstra。优先队列中的
 *   vector 依次保存（当前费用，城市，已使用次数），默认按字典序比较，因此费用最小的
 *   状态最先出队。
 *
 *   题目允许“最多”使用 k 次免费机会，最终可能使用 0..k 中的任意次数，所以答案为
 *   dist[t][0..k] 的最小值。
 *
 * 复杂度分析：
 *   - 状态数：O(n(k+1))
 *   - 转移数：O(m(k+1))
 *   - 时间复杂度：O(m(k+1) log(n(k+1)))
 *   - 空间复杂度：O(n(k+1)+m)
 */

#include <iostream>
#include <vector>
#include <climits>
#include <queue>
using namespace std;

int main() {
    // 输入规模较大，关闭同步并解除 cin 与 cout 的绑定以加快输入输出。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, s, t;
    cin >> n >> m >> k >> s >> t;
    // graph[u] 保存城市 u 的所有邻接航线，每个 pair 表示（相邻城市，价格）。
    vector<vector<pair<int, int>>> graph(n);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        // 每种航线可以双向乘坐，因此将两个方向都加入邻接表。
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }

    // dist[u][j] 表示到达 u 且恰好使用 j 次免费机会时的最小费用。
    vector dist(n, vector(k + 1, INT_MAX));
    // 队列状态依次为（费用，城市，已使用次数）；vector 按字典序构成小根堆。
    priority_queue<vector<int>, vector<vector<int>>, greater<>> pq;
    // 从 s 出发时费用为 0，尚未使用免费机会。
    dist[s][0] = 0;
    pq.push({0, s, 0});
    while (!pq.empty()) {
        auto &vt = pq.top();
        int d = vt[0], node = vt[1], used = vt[2];
        pq.pop();
        // 同一状态可能多次入队；只处理与当前最短距离相符的最新状态。
        if (d != dist[node][used]) {
            continue;
        }

        for (auto [v, w]: graph[node]) {
            // 正常支付航线价格，免费次数不变，从 (node,used) 转移到 (v,used)。
            if (dist[v][used] > d + w) {
                dist[v][used] = d + w;
                pq.push({dist[v][used], v, used});
            }
            // 如果仍有免费机会，本条航线费用记为 0，并进入下一层状态。
            if (used < k && dist[v][used + 1] > d) {
                dist[v][used + 1] = d;
                pq.push({d, v, used + 1});
            }
        }
    }

    // 免费机会不必全部使用，在到达 t 的所有层中选取最小费用。
    int minCost = INT_MAX;
    for (int i = 0; i <= k; i++) {
        minCost = min(minCost, dist[t][i]);
    }
    // 输出从 s 到 t 的最小总费用。
    cout << minCost << endl;
    return 0;
}
