/**
 * Luogu P2860 - [USACO06JAN] Redundant Paths G
 *
 * 问题描述：
 *   给定一张连通无向图，可以添加连接任意两个不同节点的新边。求至少添加多少条边，
 *   才能使任意两点之间都存在两条不共用边的路径。
 *
 * 解法：Tarjan 求桥 + 边双连通分量缩点
 *   连通无向图中存在桥时，桥两侧的节点之间都必须经过这条边，因而无法获得两条
 *   边不相交的路径。题目的目标就是添加最少的边，使图中不再存在桥。
 *
 *   1. 用 Tarjan 算法找出原图中的所有桥。题目允许重边，所以邻接表同时记录边的
 *      编号，DFS 只跳过进入当前节点时使用的同一条边。
 *   2. 忽略所有桥再次 DFS，将剩余的每个连通块染成一个边双连通分量。
 *   3. 将每个分量缩成一个点，原图中的桥就构成一棵桥树。
 *   4. 设桥树中度数为 1 的叶子数为 leaf。每条新边最多连接两个叶子分量，适当将
 *      叶子配对即可让树边进入环中，因此最少添加 ceil(leaf/2) 条边。
 *
 * 复杂度分析：
 *   Tarjan、分量染色和度数统计都只遍历常数次节点与边：
 *   - 时间复杂度：O(F+R)
 *   - 空间复杂度：O(F+R)
 */

#include <iostream>
#include <vector>
using namespace std;

// graph[u] 中的每个元素为 {相邻节点, 原无向边编号}。
vector<vector<pair<int, int>>> graph;
// dfn[u] 为 u 的访问时间；low[u] 为 u 的 DFS 子树能够到达的最小时间戳。
vector<int> dfn, low;
// bridge[id] 表示编号为 id 的无向边是否为桥。
vector<bool> bridge;
// Tarjan DFS 使用的递增时间戳。
int timer = 1;

// 从 u 开始寻找桥；pid 是进入 u 时经过的边编号。
void dfs(int u, int pid) {
    // 首次访问 u 时，只确定能够到达 u 自身。
    dfn[u] = low[u] = timer++;
    for (auto [v, id]: graph[u]) {
        // 同一条无向边会在邻接表中出现两次，跳过 DFS 树中的父边。
        // 使用边编号判断可以正确处理同一对节点之间的重边。
        if (id == pid) {
            continue;
        }
        if (dfn[v] == 0) {
            // v 尚未访问，u-v 是一条 DFS 树边。
            dfs(v, id);
            // v 的子树能够到达的位置也可以用于更新 u 的 low 值。
            low[u] = min(low[u], low[v]);

            // v 的子树无法绕过 u-v 到达 u 或 u 的祖先，因此该边是桥。
            if (low[v] > dfn[u]) {
                bridge[id] = true;
            }
        } else {
            // u-v 指向已经访问过的节点，利用这条返祖边更新 low[u]。
            low[u] = min(low[u], dfn[v]);
        }
    }
}

// component[u] 表示节点 u 所属的边双连通分量编号。
vector<int> component;

// 忽略所有桥，将与 u 连通的节点染成同一个分量编号 color。
void dfsComponent(int u, int color) {
    component[u] = color;
    for (auto [v, id]: graph[u]) {
        // 只沿非桥前进；未被染色的 v 与 u 属于同一个边双连通分量。
        if (component[v] == 0 && !bridge[id]) {
            dfsComponent(v, color);
        }
    }
}

int main() {
    int f, r;
    cin >> f >> r;
    graph.resize(f + 1);
    for (int i = 0; i < r; ++i) {
        int u, v;
        cin >> u >> v;
        // 无向边从两个方向存储，并共用同一个编号 i。
        graph[u].emplace_back(v, i);
        graph[v].emplace_back(u, i);
    }

    dfn.resize(f + 1, 0);
    low.resize(f + 1, 0);
    bridge.resize(r + 1, false);
    // 题目保证原图连通，从节点 1 出发即可访问所有节点并找出全部桥。
    dfs(1, -1);

    // componentCount 始终表示下一个可使用的分量编号。
    // 每发现一个尚未染色的节点，就从它出发建立一个新的边双连通分量。
    int componentCount = 1;
    component.resize(f + 1, 0);
    for (int u = 1; u <= f; ++u) {
        if (component[u] == 0) {
            dfsComponent(u, componentCount++);
        }
    }

    // 缩点后，每条桥连接两个不同分量。遍历桥的两个邻接表方向，恰好让桥两端
    // 所属分量的度数各增加 1，无需显式建立桥树。
    vector degree(componentCount + 1, 0);
    for (int u = 1; u <= f; ++u) {
        for (auto [v, id]: graph[u]) {
            if (bridge[id]) {
                degree[component[u]]++;
            }
        }
    }

    // 桥树中度数为 1 的分量是叶子。componentCount 已指向下一个空编号，
    // 循环多检查的空编号度数为 0，不会影响叶子数量。
    int leaf = 0;
    for (int i = 1; i <= componentCount; ++i) {
        if (degree[i] == 1) {
            leaf++;
        }
    }

    // 一条新边最多同时处理两个叶子，叶子数为奇数时向上取整。
    cout << (leaf + 1) / 2 << endl;
    return 0;
}
