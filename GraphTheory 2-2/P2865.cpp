/**
 * Luogu P2865 - [USACO06NOV] Roadblocks G
 *
 * 问题描述：
 *   给定一张带正权的无向图，求从节点 1 到节点 N 的严格次短路长度。
 *
 * 解法：维护最短路与次短路的 Dijkstra
 *   minDist[u] 和 secDist[u] 分别记录到达 u 的最短距离与严格次短距离。
 *   优先队列同时扩展这两类候选距离。用当前路径松弛边 u-v 时：
 *   - 若得到的新距离小于最短距离，原最短距离降为次短距离；
 *   - 若新距离严格位于最短和次短之间，则更新次短距离。
 *   最终 secDist[N] 即从 1 到 N 的严格次短路。
 *
 * 复杂度分析：
 *   - 时间复杂度：O((N+R) log N)
 *   - 空间复杂度：O(N+R)
 */

#include <iostream>
#include <queue>
#include <vector>
#include <climits>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, R;
    cin >> N >> R;
    vector<vector<pair<int, int>>> graph(N + 1);
    for (int i = 0; i < R; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }

    // 分别保存到每个节点的最短距离和严格次短距离
    vector minDist(N + 1, INT_MAX), secDist(N + 1, INT_MAX);
    // 小根堆中的元素为 {候选距离, 节点编号}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    minDist[1] = 0;
    pq.emplace(0, 1);
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        // 比当前次短距离还大的候选不可能再产生有效更新
        if (d > secDist[u]) {
            continue;
        }

        for (auto [v, w]: graph[u]) {
            if (minDist[v] > d + w) {
                // 新距离成为最短路，原最短路顺延为次短路
                swap(minDist[v], secDist[v]);
                minDist[v] = d + w;
                pq.emplace(minDist[v], v);
            } else if (minDist[v] < d + w && d + w < secDist[v]) {
                // 新距离必须严格大于最短路，才能作为严格次短路
                secDist[v] = d + w;
                pq.emplace(secDist[v], v);
            }
        }
    }
    cout << secDist[N] << endl;
    return 0;
}
