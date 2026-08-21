/**
 * Luogu P1194 - 买礼物
 *
 * 问题描述：
 *   需要购买 b 件礼物，单独购买任意一件的价格均为 a。已购买第 i 件礼物后，
 *   购买第 j 件礼物可以花费 K[i][j]；K[i][j]=0 表示二者之间没有优惠。求买齐
 *   所有礼物的最小花费。
 *
 * 解法：虚拟点建图 + 堆优化 Prim
 *   增设编号为 b 的虚拟点，表示直接按原价购买。虚拟点与每件礼物之间连一条权值
 *   为 a 的边；礼物 i、j 之间的边权为 K[i][j]。当 K[i][j]=0 时不存在优惠，
 *   将其边权设为 a，等价于购买该礼物时直接按原价购买。
 *
 *   一条最小生成树中的边 i-j 表示先购买其中一件，再以优惠价购买另一件；虚拟点
 *   连向某件礼物的边表示该礼物按原价购买。因此，买齐所有礼物的最小花费恰好等于
 *   这 b+1 个顶点构成的图的最小生成树权值。
 *
 *   Prim 算法从任意顶点开始扩展生成树。优先队列按照边权从小到大取出候选边，
 *   并查集用于判断候选边到达的顶点是否已经加入生成树。
 *
 * 复杂度分析：
 *   图共有 b+1 个顶点，邻接矩阵规模为 O(b^2)：
 *   - 时间复杂度：O(b^2 log b)
 *   - 空间复杂度：O(b^2)
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// 并查集维护已经加入最小生成树的顶点集合。
class UnionFind {
    // parent[x] 表示 x 在并查集中的父节点。
    vector<int> parent;

public:
    // b+1 号节点仅作为“已加入生成树”集合的代表元使用。
    UnionFind(int n) : parent(n + 2) {
        for (int i = 0; i < n + 2; i++) {
            parent[i] = i;
        }
    }

    // 查找 x 所在集合的代表元，并进行路径压缩。
    int find(int x) {
        if (parent[x] == x) {
            return x;
        }
        return parent[x] = find(parent[x]);
    }

    // 合并 x、y 所在的两个集合。
    void merge(int x, int y) {
        x = find(x);
        y = find(y);
        parent[x] = y;
    }
};

int main() {
    int a, b;
    cin >> a >> b;

    // 0 到 b-1 表示礼物，b 表示按原价购买的虚拟点。
    // graph[u][v] 保存 u、v 之间的建图边权。
    vector graph(b + 1, vector(b + 1, 0));
    for (int i = 0; i < b; i++) {
        for (int j = 0; j < b; j++) {
            cin >> graph[i][j];
            // 0 表示没有优惠，改为按原价购买所需的 a 元。
            if (i != j && graph[i][j] == 0) {
                graph[i][j] = a;
            }
        }
    }

    // 每件礼物都可以直接按原价购买，因此与虚拟点双向连一条权值为 a 的边。
    for (int i = 0; i < b; i++) {
        graph[i][b] = a;
    }
    for (int i = 0; i < b; i++) {
        graph[b][i] = a;
    }

    // sum 保存已经选入最小生成树的边权之和。
    long long sum = 0;
    UnionFind uf(b);
    // 小根堆中的 pair 表示（边权，边所到达的顶点）。
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    // 从任意礼物顶点 0 开始扩展，第一条边权为 0，不会影响答案。
    pq.emplace(0, 0);
    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();

        // 已与 b+1 号标记节点连通，说明 u 已经加入生成树。
        if (uf.find(b + 1) == uf.find(u)) {
            continue;
        }

        // 当前候选边权最小，将 u 加入生成树并累计它的边权。
        uf.merge(b + 1, u);
        sum += w;

        // 新加入 u 后，u 连出的所有边都成为后续的候选边。
        for (int i = 0; i < b + 1; i++) {
            if (i != u) {
                pq.emplace(graph[u][i], i);
            }
        }
    }

    // 图中所有 b+1 个顶点均会被加入，sum 即为最少花费。
    cout << sum << endl;
    return 0;
}
