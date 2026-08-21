/**
 * Luogu P1396 - 营救
 *
 * 问题描述：
 *   给定一张带权无向图，边权表示道路的拥挤度。需要寻找一条从 s 到 t 的路径，
 *   使路径上最大边权尽可能小，并输出这个最小值。
 *
 * 解法一：瓶颈 Dijkstra（solve1）
 *   dist[u] 表示从 s 到 u 的所有路径中，路径最大边权的最小值。若从 u 经过权值为 w
 *   的边到达 v，新路径的最大边权为 max(dist[u],w)，因此使用下面的公式松弛：
 *       dist[v] = min(dist[v], max(dist[u], w))
 *   路径继续延伸时最大边权只会保持或增大，因此可以按照 dist 从小到大确定节点。
 *
 * 解法二：堆优化 Prim（solve2，main 当前调用）
 *   从 s 开始构造最小生成树，每次选择连接当前生成树与外部顶点的最小边。最小生成树
 *   上 s 到 t 路径中的最大边权，在原图所有 s 到 t 的路径中最小。因此，当 t 第一次
 *   加入生成树时，已经选择的边权最大值就是答案。
 *
 * 复杂度分析：
 *   两种实现都会让每条邻接边进入优先队列，队列中可能保存 O(m) 个状态：
 *   - 时间复杂度：O(m log m)
 *   - 空间复杂度：O(n+m)
 */

#include <iostream>
#include <vector>
#include <climits>
#include <queue>
using namespace std;

// 解法一：使用 Dijkstra 求从 s 到每个节点的最小瓶颈值。
void solve1() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;

    // graph[u] 保存 u 的所有邻边，每个 pair 表示（相邻节点，拥挤度）。
    vector<vector<pair<int, int>>> graph(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        // 大道可以双向通行，因此加入两个方向。
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }

    // dist[u] 表示从 s 到 u 的路径中，最大边权的最小值。
    vector dist(n + 1, INT_MAX);
    // 小根堆中的 pair 表示（当前瓶颈值，节点）。
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    // 从 s 到自身不经过任何道路，初始瓶颈值记为 0。
    dist[s] = 0;
    pq.emplace(0, s);
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // 同一节点可能被多次更新并入队，跳过已经失效的旧状态。
        if (d != dist[u]) {
            continue;
        }

        for (auto [v, w]: graph[u]) {
            // 经 u 到达 v 后，新路径的瓶颈值为已有瓶颈值与当前边权的较大者。
            if (dist[v] > max(dist[u], w)) {
                dist[v] = max(dist[u], w);
                pq.emplace(dist[v], v);
            }
        }
    }

    // dist[t] 即为所有 s 到 t 路径中最小的最大拥挤度。
    cout << dist[t] << endl;
}

// 解法二：从 s 开始运行 Prim，直到 t 加入最小生成树。
void solve2() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;

    // 使用邻接表保存带权无向图。
    vector<vector<pair<int, int>>> graph(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }

    // 并查集将所有已加入生成树的节点与标记节点 0 合并。
    class UnionFind {
        // parent[x] 表示 x 在并查集中的父节点。
        vector<int> parent;

    public:
        // 初始化时每个节点各自属于一个集合。
        UnionFind(int n) : parent(n + 1) {
            for (int i = 0; i < n + 1; ++i) {
                parent[i] = i;
            }
        }

        // 查找集合代表元，并进行路径压缩。
        int find(int x) {
            if (parent[x] != x) {
                parent[x] = find(parent[x]);
            }
            return parent[x];
        }

        // 合并 x、y 所在的两个集合。
        void merge(int x, int y) {
            x = find(x);
            y = find(y);
            parent[x] = y;
        }
    };

    // res 保存 Prim 到目前为止选择过的最大边权。
    int res = 0;
    UnionFind uf(n + 1);
    // 小根堆中的 pair 表示（候选边权，边所到达的节点）。
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    // 从 s 开始扩展，第一条虚拟边权为 0，不影响答案。
    pq.emplace(0, s);
    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();

        // 与标记节点 0 连通，说明 u 已经加入生成树。
        if (uf.find(u) == uf.find(0)) {
            continue;
        }

        // 选择当前最小的有效候选边，将 u 加入生成树。
        uf.merge(u, 0);
        res = max(res, w);

        // t 第一次加入时，生成树上 s 到 t 路径的瓶颈值已经确定。
        if (uf.find(s) == uf.find(t)) {
            cout << res << endl;
            return;
        }

        // u 的所有邻边都可能成为后续扩展生成树的候选边。
        for (auto [v, vw]: graph[u]) {
            pq.emplace(vw, v);
        }
    }
}

int main() {
    // 当前选择堆优化 Prim；改为 solve1() 即可使用瓶颈 Dijkstra。
    solve2();
    return 0;
}
