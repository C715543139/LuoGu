/**
 * Luogu P3366 - 【模板】最小生成树
 *
 * 问题描述：
 *   给定一个包含 n 个顶点、m 条带权无向边的图，求一棵最小生成树，并输出其中
 *   所有边权之和。如果原图不连通，无法形成生成树，则输出 "orz"。
 *
 * 解法：堆优化 Prim
 *   从顶点 1 开始扩展生成树。优先队列保存由当前生成树中的顶点连出的候选边，
 *   并按照边权从小到大取出。若候选边到达的顶点 u 尚未加入生成树，根据 Prim
 *   算法的贪心性质，这条边就是连接当前生成树与 u 的最小边，可以计入答案。
 *
 *   并查集维护已经加入生成树的顶点：所有已加入的顶点都与顶点 1 属于同一集合。
 *   如果 u 已与顶点 1 连通，说明该候选边的终点已经在生成树中，跳过即可；否则
 *   将 u 与顶点 1 合并，并把 u 的所有邻边加入优先队列，继续扩展生成树。
 *
 *   优先队列耗尽后，再检查每个顶点是否都与顶点 1 连通。存在未连通顶点时，说明
 *   原图不连通；否则累计的边权和就是最小生成树的权值。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(m log m + n alpha(n))，通常记作 O(m log m)
 *   - 空间复杂度：O(n+m)
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// 并查集用于判断一个顶点是否已经加入生成树。
class U {
    // vt[x] 表示 x 在并查集中的父节点。
    vector<int> vt;

public:
    // 初始时每个顶点各自属于一个集合。
    U(int n) : vt(n + 1) {
        for (int i = 0; i <= n; ++i) vt[i] = i;
    }

    // 查找 x 所在集合的代表元，并进行路径压缩。
    int find(int x) {
        if (x == vt[x]) {
            return x;
        }
        vt[x] = find(vt[x]);
        return vt[x];
    }

    // 合并 x、y 所在的两个集合。
    void merge(int x, int y) {
        int rx = find(x), ry = find(y);
        vt[rx] = ry;
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    // graph[u] 保存与 u 相连的边，每个 pair 表示（相邻顶点，边权）。
    vector<vector<pair<int, int>>> graph(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        // 题目给出无向边，因此需要加入两个方向。
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }

    U us(n);
    // sum 保存当前已经选入生成树的边权之和，使用 long long 防止累加溢出。
    long long sum = 0;

    // 小根堆中的 pair 表示（边权，边所到达的顶点）。
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    // Prim 从顶点 1 开始，将它的所有邻边放入候选集合。
    for (auto [v, w]: graph[1]) {
        pq.emplace(w, v);
    }

    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();

        // 与顶点 1 同属一个集合，说明 u 已在生成树中，这条边会形成环。
        if (us.find(1) == us.find(u)) {
            continue;
        }

        // 选择当前权值最小的有效候选边，并将 u 加入生成树。
        sum += w;
        us.merge(1, u);

        // u 加入后，它的所有邻边都可能成为后续扩展生成树的候选边。
        for (auto [v, vw]: graph[u]) {
            pq.emplace(vw, v);
        }
    }

    // 存在未加入生成树的顶点，说明原图不连通，无法构造生成树。
    for (int i = 2; i <= n; ++i) {
        if (us.find(1) != us.find(i)) {
            cout << "orz" << endl;
            return 0;
        }
    }

    // 所有顶点均已加入，sum 即为最小生成树的边权之和。
    cout << sum << endl;
    return 0;
}
