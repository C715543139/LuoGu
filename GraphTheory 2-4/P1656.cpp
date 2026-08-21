/**
 * Luogu P1656 - 炸铁路
 *
 * 问题描述：
 *   给定一张连通无向图，寻找所有删除后会使图不再连通的边，并按照两个端点的编号
 *   从小到大输出。这类边在图论中称为桥或割边。
 *
 * 解法：Tarjan 求桥
 *   对图进行深度优先搜索，dfn[u] 记录节点 u 第一次被访问的时间；low[u] 记录从 u
 *   的 DFS 子树出发，经过树边和至多一条返祖边能够到达的最早节点的访问时间。
 *
 *   假设 DFS 通过树边 u-v 首次到达 v：
 *   - 若 low[v]<=dfn[u]，说明 v 的子树可以绕过 u-v 回到 u 或 u 的祖先；
 *   - 若 low[v]>dfn[u]，说明 u-v 是 v 的子树与图中其余部分的唯一通道，因此是桥。
 *
 * 复杂度分析：
 *   每个节点访问一次，每条无向边从两个方向各检查一次：
 *   - Tarjan 时间复杂度：O(n+m)
 *   - 答案排序复杂度：O(k log k)，k 为桥的数量且 k<=n-1
 *   - 空间复杂度：O(n+m)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// timer 为 DFS 时间戳；dfn[u]=0 表示 u 尚未访问。
int timer = 1;
// dfn[u] 是 u 的访问时间，low[u] 是 u 的子树能够到达的最小时间戳。
vector<int> dfn, low;
// graph 保存无向图，bridges 保存所有桥的两个端点。
vector<vector<int>> graph;
vector<pair<int, int>> bridges;

// 从 u 开始 DFS，parent 是 u 在 DFS 树中的父节点。
void dfs(int u, int parent) {
    // 首次访问 u 时，能够确定到达的最早节点就是 u 自身。
    dfn[u] = low[u] = timer++;
    for (int v : graph[u]) {
        // 无向边会在邻接表中出现两次，跳过进入 u 时经过的父边。
        // 题目保证没有重边，因此可以直接使用父节点编号判断。
        if (v == parent) {
            continue;
        }
        if (dfn[v] == 0) {
            // v 尚未访问，u-v 是一条 DFS 树边。
            dfs(v, u);
            // v 的子树能够到达的最早节点也可以用于更新 u。
            low[u] = min(low[u], low[v]);
        } else if (dfn[v] != 0) {
            // v 已经访问，利用这条返祖边更新 u 能够到达的最早时间戳。
            low[u] = min(low[u], dfn[v]);
        }

        // v 的子树无法绕过 u-v 回到 u 或更早的祖先，因此 u-v 是桥。
        if (low[v] > dfn[u]) {
            // 每条边统一保存为较小端点在前，便于最后按题意排序。
            bridges.emplace_back(min(u, v), max(u, v));
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;

    dfn.resize(n + 1);
    low.resize(n + 1);
    graph.resize(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // 题目保证原图连通，从节点 1 开始即可访问所有节点。
    dfs(1, 0);

    // 按第一端点升序、第二端点升序排列所有桥。
    sort(bridges.begin(), bridges.end());
    for (auto [u, v] : bridges) {
        cout << u << ' ' << v << '\n';
    }
    return 0;
}
