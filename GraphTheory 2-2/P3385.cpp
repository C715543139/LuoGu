/**
 * Luogu P3385 - 【模板】负环
 *
 * 问题描述：
 *   给定一个含 n 个顶点的图，判断是否存在从顶点 1 出发能够到达的负环。
 *   输入的边权 w 为非负数时，u-v 表示一组同权双向边；w 为负数时，
 *   只有一条 u -> v 的有向边。
 *
 * 解法：SPFA 判定负环
 *   从顶点 1 开始运行 SPFA，dist[v] 记录当前已知的 1 到 v 的最短距离。
 *   每当边 u -> v 使 dist[v] 变小，就用 edgeCount[u]+1 记录这条新路径
 *   包含的边数，并将 v 放入队列，继续用它更新后继顶点。
 *
 *   不含重复顶点的路径最多只有 n-1 条边。如果一条能够继续缩短距离的
 *   路径已经包含至少 n 条边，其中必然重复经过某个顶点并形成环；该环使
 *   路径距离继续减小，因此它是从顶点 1 可达的负环。
 *
 * 复杂度分析：
 *   - 最坏时间复杂度：O(nE)，E 为按题意建图后的实际有向边数
 *   - 空间复杂度：O(n+E)
 */

#include <iostream>
#include <queue>
#include <vector>
#include <climits>
using namespace std;

int main() {
    // 题目包含多组测试数据。
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;

        // graph[u] 保存 u 的所有出边，pair 表示（终点，边权）。
        vector<vector<pair<int, int>>> graph(n + 1);
        for (int i = 0; i < m; i++) {
            int u, v, w;
            cin >> u >> v >> w;

            // 每条边信息都至少产生一条 u -> v 的有向边。
            graph[u].emplace_back(v, w);
            // 非负权边按题意可以双向通行，因此再加入反向边。
            if (w >= 0) {
                graph[v].emplace_back(u, w);
            }
        }

        // flag 表示当前还没有发现从 1 可达的负环。
        bool flag = true;
        queue<int> q;
        // inQueue[u] 表示 u 当前是否在队列中，避免重复入队。
        vector inQueue(n + 1, false);
        // dist[u] 是从 1 到 u 的当前最短距离；edgeCount[u] 是该路径包含的边数。
        vector dist(n + 1, INT_MAX), edgeCount(n + 1, 0);
        // 只从顶点 1 出发，因此只会检测到从 1 能够到达的负环。
        dist[1] = 0;
        q.push(1);
        while (!q.empty() && flag) {
            int u = q.front();
            // u 离开队列后，之后距离再次变小时可以重新入队。
            inQueue[u] = false;
            q.pop();
            for (auto [v, w]: graph[u]) {
                // 尝试通过边 u -> v 缩短从 1 到 v 的距离。
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    // 新路径先到达 u，再多经过一条边到达 v。
                    edgeCount[v] = edgeCount[u] + 1;
                    // v 的距离变小，需要继续用 v 松弛它的出边。
                    if (!inQueue[v]) {
                        q.push(v);
                        inQueue[v] = true;
                    }

                    // 能够继续缩短距离的路径含至少 n 条边，说明其中存在负环。
                    if (edgeCount[v] >= n) {
                        cout << "YES\n";
                        flag = false;
                        break;
                    }
                }
            }
        }
        // 队列耗尽仍未触发负环判定，说明从 1 可达的部分中没有负环。
        if (flag) {
            cout << "NO\n";
        }
    }
    return 0;
}
