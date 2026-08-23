/**
 * Codeforces 1000E - We Need More Bosses
 *
 * 问题描述：
 *   给定一张连通无向图，选择两个节点 s、t，使从 s 到 t 的每条路径都必须经过的
 *   边数最多，输出这个最大值。
 *
 * 解法：Tarjan 缩边双连通分量 + 桥树直径
 *   一条边被 s 到 t 的所有路径共同经过，当且仅当它是原图中的桥。删去所有桥后，
 *   每个剩余连通块缩成一个点；原图中的桥连接这些缩点，得到一棵桥树。
 *   两点之间必经的桥数恰好等于其对应缩点在桥树上的距离，因此答案就是桥树直径。
 *
 *   1. 使用 Tarjan 的 dfn、low 和栈，将被桥分隔的节点划入不同分量。
 *   2. 将不同分量之间的原边加入桥树。
 *   3. 从桥树任意节点出发找到最远点，再从该最远点出发找到最远点；第二次距离
 *      就是树的直径。
 *
 * 复杂度分析：
 *   每个节点和每条边只会被 Tarjan、建树和两次 DFS 常数次访问：
 *   - 时间复杂度：O(n+m)
 *   - 空间复杂度：O(n+m)
 */

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// graph[u] 中的每个元素为 {相邻节点, 原无向边编号}。
vector<vector<pair<int, int>>> graph;
// dfn、low 分别记录 Tarjan 时间戳和能够回到的最小时间戳。
// scc[u] 表示 u 所属的边双连通分量编号。
vector<int> dfn, low, stack, scc;
// processing[u] 表示 u 当前仍在 Tarjan 栈中。
vector<bool> processing;
// timer 为递增时间戳；sccCount 始终表示下一个可分配的分量编号。
int timer = 1, sccCount = 1;

// 从 u 开始 Tarjan DFS；pid 是进入 u 时经过的无向边编号。
void dfs(int u, int pid) {
    // 新节点入栈，并初始化 dfn 与 low。
    processing[u] = true;
    stack.push_back(u);
    dfn[u] = low[u] = timer++;
    for (auto [v , id]: graph[u]) {
        // 同一条无向边会在邻接表中出现两次，只跳过进入 u 时经过的父边。
        // 通过边编号判断可以正确处理重边。
        if (id == pid) {
            continue;
        }
        if (dfn[v] == 0) {
            // v 尚未访问，u-v 是 DFS 树边。
            dfs(v, id);
            // v 的子树能够到达的最早节点也可以用于更新 u。
            low[u] = min(low[u], low[v]);
        } else if (processing[v]) {
            // v 仍在栈中，u-v 能将 u 回连到栈内祖先或同一分量中的节点。
            low[u] = min(low[u], dfn[v]);
        }
    }

    // u 是当前分量中 dfn 最小的节点，持续出栈直到 u，得到一个边双连通分量。
    if (dfn[u] == low[u]) {
        int top;
        do {
            top = stack.back();
            stack.pop_back();
            processing[top] = false;
            scc[top] = sccCount;
        } while (top != u);
        sccCount++;
    }
}

// newGraph 是缩点后的桥树。
vector<vector<int>> newGraph;
// 两次求树直径时记录本次 DFS 已访问的桥树节点。
vector<bool> visited;
// maxDepth 为当前 DFS 找到的最远距离；treeRoot 为对应的最远节点。
int maxDepth = 0, treeRoot;

// 在桥树中从 root 出发深搜，更新最远叶子及其距离。
void dfsDepth(int u, int depth, int root) {
    visited[u] = true;
    // 除起点外，叶子一定可能成为树直径的端点。
    if (newGraph[u].size() == 1 && u != root) {
        if (maxDepth < depth) {
            maxDepth = depth;
            treeRoot = u;
        }
        return;
    }
    for (int v: newGraph[u]) {
        if (!visited[v]) {
            // 桥树无环，沿未访问边继续累计经过的桥数。
            dfsDepth(v, depth + 1, root);
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    // 无向边从两个方向存储，两个方向共用输入时的边编号 i。
    graph.assign(n + 1, {});
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].emplace_back(v, i);
        graph[v].emplace_back(u, i);
    }

    dfn.assign(n + 1, 0);
    low.assign(n + 1, 0);
    scc.assign(n + 1, 0);
    processing.assign(n + 1, false);
    // 题目保证原图连通，从节点 1 出发即可完成缩点。
    dfs(1, 0);

    // add[id] 保证每条无向边只参与一次桥树建边。
    vector add(m + 1, false);
    newGraph.assign(sccCount, {});
    // 这里遍历的是原图节点，而非分量编号；不同分量之间的边就是桥树边。
    for (int u = 1; u <= n; u++) {
        for (auto [v, id]: graph[u]) {
            int U = scc[u];
            int V = scc[v];
            if (U != V && !add[id]) {
                newGraph[U].push_back(V);
                newGraph[V].push_back(U);
            }
            add[id] = true;
        }
    }

    // 第一次 DFS 从任意桥树节点 1 出发，treeRoot 记录其最远叶子。
    visited.assign(sccCount, false);
    dfsDepth(1, 0, 1);
    // 第二次 DFS 从该最远叶子出发，得到的最远距离即桥树直径。
    visited.assign(sccCount, false);
    dfsDepth(treeRoot, 0, treeRoot);
    cout << maxDepth << endl;
    return 0;
}
