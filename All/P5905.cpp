/**
 * Luogu P5905 - 【模板】Johnson 全源最短路
 *
 * 问题描述：
 *   给定一张可能含负权边的有向图。若图中存在负环则输出 -1；否则对每个起点
 *   i，输出它到所有节点 j 的最短距离乘 j 后的总和。不可达距离按 10^9 计算。
 *
 * 解法：Johnson 算法
 *   先把所有势能 h 初始化为 0，这等价于增加一个到所有节点均有零权边的超级源点，
 *   再进行 Bellman-Ford 松弛。若第 n 轮仍能更新，则原图存在负环。
 *   随后把边权重标为 w'=w+h[u]-h[v]，此时所有边权均非负，可以从每个节点运行
 *   Dijkstra。由重标后的距离还原原距离：dist'(i,j)+h[j]-h[i]。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(nm + nm log n)
 *   - 空间复杂度：O(n+m)
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector graph(n + 1, vector<pair<int, int>>{});
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].emplace_back(v, w);
    }

    // h 是 Johnson 重标边权所需的势能；全零初始化等价于超级源点的初始距离
    vector h(n + 1, 0);
    // Bellman-Ford：进行至多 n 轮全边松弛，并在第 n 轮判断负环
    for (int i = 1; i <= n; i++) {
        bool update = false;
        for (int j = 1; j <= n; j++) {
            for (auto [v, w]: graph[j]) {
                if (h[v] > h[j] + w) {
                    h[v] = h[j] + w;
                    update = true;
                }
            }
        }
        if (update && i == n) {
            cout << -1 << endl;
            return 0;
        }
    }

    // 依次以每个节点为源点，在重标后的非负权图上运行 Dijkstra
    for (int i = 1; i <= n; i++) {
        vector dist(n + 1, 1000000000);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.emplace(0, i);
        dist[i] = 0;

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            // 丢弃节点 u 已被更短路径替代的旧堆元素
            if (dist[u] != d) {
                continue;
            }

            for (auto [v, w]: graph[u]) {
                // Johnson 重标：w' = w + h[u] - h[v]
                w += h[u] - h[v];
                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    pq.emplace(dist[v], v);
                }
            }
        }

        // 将重标后的距离还原；不可达节点按照题意使用 10^9
        long long sum = 0;
        for (int j = 1; j <= n; j++) {
            sum += 1ll * (dist[j] == 1000000000 ? 1000000000 : dist[j] + h[j] - h[i]) * j;
        }
        cout << sum << '\n';
    }
    return 0;
}
