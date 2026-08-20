/**
 * Luogu P5960 - 【模板】差分约束
 *
 * 问题描述：
 *   给定 n 个未知数和 m 个形如 x_u-x_v<=w 的不等式，求任意一组满足
 *   所有不等式的解；如果约束之间存在矛盾，则输出 NO。
 *
 * 解法：差分约束建图 + SPFA
 *   将不等式
 *       x_u-x_v<=w
 *   移项得到
 *       x_u<=x_v+w。
 *   这与最短路中边 v -> u 的松弛条件 dist[u]<=dist[v]+w 形式相同，
 *   因此每个不等式都转化为一条从 v 到 u、边权为 w 的有向边。
 *
 *   为了使所有未知数都能参与松弛，增加超级源点 0，并向每个顶点 i
 *   连一条权值为 0 的边。从 0 运行 SPFA 后，dist[i] 满足图中所有边对应的
 *   dist[u]<=dist[v]+w，所以可直接将 dist[1..n] 作为一组可行解输出。
 *
 *   如果图中存在负环，环上距离可以被无限次缩短，说明相应约束无法
 *   同时成立。图中共有 n+1 个顶点；若某条仍在缩短距离的路径已经包含
 *   至少 n+1 条边，其中必然包含负环，此时输出 NO。
 *
 * 复杂度分析：
 *   - 顶点数为 n+1，边数为 m+n
 *   - 最坏时间复杂度：O(n(n+m))
 *   - 空间复杂度：O(n+m)
 */

#include <iostream>
#include <vector>
#include <climits>
#include <queue>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    // graph[a] 保存 a 的所有出边，pair 表示（终点，边权）。
    vector<vector<pair<int, int>>> graph(n + 1);
    // 建立超级源点 0，用权值为 0 的边连向每个变量顶点。
    for (int i = 1; i <= n; ++i) {
        graph[0].emplace_back(i, 0);
    }
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        // x_u-x_v<=w 等价于 x_u<=x_v+w，对应有向边 v -> u，边权为 w。
        graph[v].emplace_back(u, w);
    }

    queue<int> q;
    // inQueue[u] 表示顶点 u 当前是否已在队列中。
    vector inQueue(n + 1, false);
    // dist[u] 是超级源点到 u 的当前最短距离，edgeCount[u] 记录该路径的边数。
    vector dist(n + 1, INT_MAX), edgeCount(n + 1, 0);
    // 从超级源点开始 SPFA，使每个变量顶点都可达。
    q.push(0);
    dist[0] = 0;
    while (!q.empty()) {
        int u = q.front();
        // u 离开队列后，如果它的距离再次变小，仍可重新入队。
        inQueue[u] = false;
        q.pop();

        for (auto [v, w] : graph[u]) {
            // 尝试用约束 dist[v]<=dist[u]+w 缩短 v 的当前距离。
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                // 新路径先到达 u，再经过一条边到达 v。
                edgeCount[v] = edgeCount[u] + 1;

                // v 的距离变小，需要继续用 v 松弛它的出边。
                if (!inQueue[v]) {
                    q.push(v);
                    inQueue[v] = true;
                }

                // 图中有 n+1 个顶点；可继续缩短的路径达到 n+1 条边时必然含负环。
                if (edgeCount[v] >= n + 1) {
                    cout << "NO" << endl;
                    return 0;
                }
            }
        }
    }

    // 无负环时，dist[1..n] 满足全部差分约束，可作为一组可行解。
    for (int i = 1; i <= n; ++i) {
        cout << dist[i] << ' ';
    }
    return 0;
}
