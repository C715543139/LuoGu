/**
 * Luogu P4779 - 【模板】单源最短路径（标准版）
 *
 * 问题描述：
 *   给定一个包含 n 个节点、m 条有向边的非负权图，求起点 s 到每个节点的最短距离。
 *   题目保证从 s 出发可以到达所有节点。
 *
 * 解法：堆优化 Dijkstra
 *   dist[u] 保存当前已知的 s 到 u 的最短距离上界。优先队列按照距离从小到大取出
 *   候选节点；当节点 u 第一次作为未访问节点出队时，由于所有边权非负，不可能再通过
 *   后续节点找到一条到 u 的更短路径，因此此时 dist[u] 已经确定。
 *
 *   确定 u 后枚举它的所有出边 u -> v。如果经过 u 到达 v 的距离
 *       dist[u] + w
 *   小于当前的 dist[v]，就更新 dist[v]，并把新的候选状态放入优先队列。一次更新不会
 *   删除队列中的旧状态，因此同一节点可能多次入队；visited 用于跳过这些已经失效的状态。
 *
 *   主循环恰好确定 n 个节点。题目保证所有节点均可达，所以每轮都能从优先队列中找到
 *   一个尚未确定最短距离的节点。
 *
 * 复杂度分析：
 *   - 时间复杂度：O((n+m) log m)，通常记作 O((n+m) log n)
 *   - 空间复杂度：O(n+m)
 */

#include <iostream>
#include <queue>
#include <vector>
#include <climits>
using namespace std;

int main() {
    // 输入规模较大，关闭同步并解除 cin 与 cout 的绑定以加快输入输出。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s;
    cin >> n >> m >> s;
    // graph[u] 保存 u 的所有出边，每个 pair 表示（终点，边权）。
    vector<vector<pair<int, int>>> graph(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        // 题目给出的是有向边，只加入 u -> v。
        graph[u].emplace_back(v, w);
    }

    // dist 初始为无穷大；visited[u] 表示 u 的最短距离是否已经确定。
    vector dist(n + 1, INT_MAX);
    vector visited(n + 1, false);
    dist[s] = 0;

    // 小根堆中的 pair 表示（候选距离，节点），距离较小的状态优先出队。
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.emplace(0, s);
    for (int i = 1; i <= n; i++) {
        auto node = pq.top();
        pq.pop();
        // 同一节点可能因多次松弛而重复入队，跳过已经确定过的旧状态。
        while (visited[node.second]) {
            node = pq.top();
            pq.pop();
        }

        // 当前未访问节点具有全局最小的候选距离，其最短距离在此确定。
        visited[node.second] = true;
        for (auto [child, w]: graph[node.second]) {
            // 尝试用 s -> node -> child 这条路径缩短 child 的当前距离。
            if (dist[child] > dist[node.second] + w) {
                dist[child] = dist[node.second] + w;
                // 保留新的候选状态；旧状态之后会由 visited 判定并跳过。
                pq.emplace(dist[child], child);
            }
        }
    }

    // 按节点编号输出从 s 出发的最短距离。
    for (int i = 1; i <= n; i++) {
        cout << dist[i] << ' ';
    }
    return 0;
}
