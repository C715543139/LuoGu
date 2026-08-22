/**
 * Luogu P2863 - [USACO06JAN] The Cow Prom S
 *
 * 问题描述：
 *   给定一张有向图，求节点数量大于 1 的强连通分量个数。强连通分量中的任意两个
 *   节点都可以沿有向边相互到达。
 *
 * 解法：Tarjan 求强连通分量
 *   dfn[u] 记录节点 u 第一次被访问的时间；low[u] 记录从 u 出发，沿 DFS 树边
 *   和指向当前栈中节点的边能够到达的最小时间戳。
 *
 *   DFS 首次访问节点时将它压入栈，并用 processing[u] 标记它仍在等待确定所属的
 *   强连通分量。遍历有向边 u->v 时：
 *   - 若 v 尚未访问，递归处理 v，再用 low[v] 更新 low[u]；
 *   - 若 v 仍在栈中，u 可以到达当前搜索路径中的节点，用 dfn[v] 更新 low[u]；
 *   - 若 v 已经出栈，它所属的强连通分量已经确定，不参与 low[u] 的更新。
 *
 *   当 dfn[u]==low[u] 时，u 是一个强连通分量在 DFS 树中的根。不断弹栈直到 u，
 *   弹出的全部节点恰好组成一个强连通分量；其节点数大于 1 时计入答案。
 *
 * 复杂度分析：
 *   每个节点入栈、出栈各一次，每条有向边检查一次：
 *   - 时间复杂度：O(n+m)
 *   - 空间复杂度：O(n+m)
 */

#include <iostream>
#include <vector>
using namespace std;

// graph[u] 中的每个元素为 {有向边终点, 边编号}，只保存 u 指向终点的方向。
vector<vector<pair<int, int>>> graph;
// dfn[u] 为 u 的访问时间；low[u] 为 u 能够到达的栈内节点中的最小时间戳。
// stack 保存尚未划入强连通分量的节点。
vector<int> dfn, low, stack;
// processing[u] 表示 u 当前是否仍在 Tarjan 栈中。
vector<bool> processing;
// timer 为 DFS 时间戳；count 为节点数大于 1 的强连通分量数量。
int timer = 1, count = 0;

// 从 u 开始进行 Tarjan DFS；pid 是进入 u 时经过的边编号。
void dfs(int u, int pid) {
    // 新访问的节点进入栈，等待确定所属的强连通分量。
    stack.push_back(u);
    processing[u] = true;
    dfn[u] = low[u] = timer++;
    for (auto [v, id]: graph[u]) {
        // 有向边只保存正向，父边通常不会出现在 u 的出边中；保留该判断以符合
        // 当前函数传递父边编号的写法。
        if (id == pid) {
            continue;
        }

        if (dfn[v] == 0) {
            // v 尚未访问，先递归求出 v 的 low 值，再更新 u。
            dfs(v, id);
            low[u] = min(low[u], low[v]);
        } else if (processing[v]) {
            // v 已访问且仍在栈中，u->v 可以让 u 到达时间戳更小的栈内节点。
            low[u] = min(low[u], dfn[v]);
        }
        // v 已经出栈时，其强连通分量已经确定，不能再用于更新 low[u]。
    }

    // u 无法到达时间戳更小的栈内节点，因此 u 是一个强连通分量的根。
    if (dfn[u] == low[u]) {
        int nodeCount = 0;
        // 弹出 u 上方的所有节点，它们与 u 属于同一个强连通分量。
        while (stack.back() != u) {
            nodeCount++;
            processing[stack.back()] = false;
            stack.pop_back();
        }
        // 循环在栈顶为 u 时停止，最后将 u 自身计数并弹出。
        nodeCount++;
        processing[u] = false;
        stack.pop_back();
        // bool 会转换为 0 或 1，只统计包含至少两个节点的强连通分量。
        count += nodeCount > 1;
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    graph.resize(n + 1);
    for (int i = 1; i <= m; ++i) {
        int u, v;
        cin >> u >> v;
        // 题目给出有向边 u->v，因此只加入 u 的邻接表。
        graph[u].emplace_back(v, i);
    }

    dfn.resize(n + 1, 0);
    low.resize(n + 1, 0);
    processing.resize(n + 1, false);
    // 图不一定从一个节点就能访问完整；从每个尚未访问的节点启动 Tarjan DFS。
    for (int u = 1; u <= n; ++u) {
        if (dfn[u] == 0) {
            dfs(u, 0);
        }
    }
    // 输出节点数量大于 1 的强连通分量总数。
    cout << count << endl;
    return 0;
}
