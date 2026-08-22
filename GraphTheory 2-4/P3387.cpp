/**
 * Luogu P3387 - 【模板】缩点 / 强连通分量
 *
 * 问题描述：
 *   给定一张带点权的有向图，寻找一条路径，使路径上不同节点的点权之和最大。
 *   路径可以重复经过边和节点，但每个节点的权值只计算一次。
 *
 * 解法：Tarjan 强连通分量缩点 + DAG 上记忆化搜索
 *   1. 在同一个强连通分量内，可以通过重复行走访问其中所有节点，再从需要的出口
 *      离开。因此将每个强连通分量缩成一个点，其点权为分量内所有原节点权值之和。
 *   2. 不同强连通分量之间的原图边构成缩点图；缩点图中不可能存在有向环，所以它
 *      是一张 DAG。
 *   3. 令 memory[u] 表示从缩点 u 出发能够取得的最大点权和，则叶子状态为
 *      memory[u]=newW[u]，其余状态为
 *      memory[u]=newW[u]+max(memory[v])，其中 u->v 是缩点图中的边。
 *
 *   本题所有点权非负，所以有出边时继续走不会使总权值减小；最终取所有缩点起点
 *   对应状态的最大值即可。
 *
 * 复杂度分析：
 *   Tarjan、缩点和 DAG 搜索都只遍历常数次节点与边：
 *   - 时间复杂度：O(n+m)
 *   - 空间复杂度：O(n+m)
 */

#include <iostream>
#include <vector>
using namespace std;

// graph[u] 保存原有向图中从 u 出发的所有边的终点。
vector<vector<int>> graph;
// w[u] 是原节点权值；dfn、low 用于 Tarjan；stack 是 Tarjan 栈；
// scc[u] 是原节点 u 所属的强连通分量编号。
vector<int> w, dfn, low, stack, scc;
// processing[u] 表示 u 是否仍在 Tarjan 栈中，尚未确定最终所属分量。
vector<bool> processing;
// timer 为 DFS 时间戳；color 是当前要分配的强连通分量编号。
int timer = 1, color = 1;

// 从原图节点 u 开始进行 Tarjan DFS，划分强连通分量。
void dfs(int u) {
    // 首次访问 u：将其标记为栈内节点并记录访问时间。
    processing[u] = true;
    stack.push_back(u);
    dfn[u] = low[u] = timer++;

    for (int v: graph[u]) {
        if (dfn[v] == 0) {
            // v 尚未访问，递归后用 v 的 low 值更新 u。
            dfs(v);
            low[u] = min(low[u], low[v]);
        } else if (processing[v]) {
            // v 已访问且仍在栈中，利用边 u->v 更新 u 能到达的最小时间戳。
            low[u] = min(low[u], dfn[v]);
        }
        // v 已经出栈时，其强连通分量已经确定，不再用于更新 low[u]。
    }

    // u 无法到达时间戳更小的栈内节点，因此 u 是一个强连通分量的根。
    if (dfn[u] == low[u]) {
        int top;
        // 从栈顶不断弹出节点，直到弹出 u；这些节点属于同一个强连通分量。
        do {
            top = stack.back();
            stack.pop_back();
            scc[top] = color;
            processing[top] = false;
        } while (top != u);
        // 下一个强连通分量使用新的编号。
        color++;
    }
}

// newGraph 是强连通分量缩点后得到的 DAG。
vector<vector<int>> newGraph;
// newW[u] 是分量 u 的权值总和；memory[u] 是从分量 u 出发的最大路径权值。
vector<int> newW, memory;
// res 保存所有可能起点对应的最大答案；全局变量初始值为 0。
int res;

// 在缩点 DAG 上计算从分量 u 出发能够取得的最大点权和。
void dfsSum(int u) {
    // 没有后继分量时，路径只能在 u 结束。
    if (newGraph[u].empty()) {
        memory[u] = newW[u];
        res = max(res, memory[u]);
        return;
    }
    // memory[u] 已经计算完成，直接复用，避免重复搜索相同子问题。
    if (memory[u] != -1) {
        return;
    }

    for (int v: newGraph[u]) {
        // 先计算后继状态，再选择能够产生最大权值和的出边。
        dfsSum(v);
        memory[u] = max(memory[u], newW[u] + memory[v]);
        res = max(res, memory[u]);
    }
}


int main() {
    int n, m;
    cin >> n >> m;
    w.resize(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        // 读取原图每个节点的点权。
        cin >> w[i];
    }
    graph.resize(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        // 题目给出有向边 u->v，因此只加入 u 的邻接表。
        graph[u].push_back(v);
    }

    dfn.resize(n + 1, 0);
    low.resize(n + 1, 0);
    scc.resize(n + 1, 0);
    processing.resize(n + 1, false);
    // 原图可能无法从一个节点全部访问，从每个未访问节点启动一次 Tarjan DFS。
    for (int u = 1; u <= n; u++) {
        if (dfn[u] == 0) {
            dfs(u);
        }
    }

    // Tarjan 结束后 color 指向下一个空编号，已有分量编号范围为 [1,color-1]。
    newGraph.resize(color);
    newW.resize(color, 0);
    for (int u = 1; u <= n; u++) {
        int uColor = scc[u];
        // 将原节点 u 的权值累加到它所属的强连通分量。
        newW[uColor] += w[u];
        for (int v: graph[u]) {
            int vColor = scc[v];
            // 跨分量的原图边成为缩点图中的边；分量内部的边直接忽略。
            if (uColor != vColor) {
                newGraph[uColor].push_back(vColor);
            }
        }
    }

    // -1 表示该分量的最大路径权值尚未计算；合法答案均不小于 0。
    memory.resize(color, -1);
    // 缩点图可能有多个互不相连的部分，从每个尚未计算的分量开始搜索。
    for (int u = 1; u < color; u++) {
        if (memory[u] == -1) {
            dfsSum(u);
        }
    }
    // 输出所有起点中能够取得的最大点权和。
    cout << res << endl;
    return 0;
}
