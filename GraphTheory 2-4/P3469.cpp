/**
 * Luogu P3469 - [POI 2008] BLO-Blockade
 *
 * 问题描述：
 *   对每个节点 u，删除所有与 u 相连的边，但保留节点 u 本身。求此时所有不连通
 *   有序点对 (x, y) 的数量。
 *
 * 解法：Tarjan 求割点分隔的子树大小
 *   删除 u 的关联边后，u 会单独成为一个大小为 1 的连通块。对于 DFS 树边 u-v，
 *   若 low[v]>=dfn[u]，则 v 的整棵 DFS 子树无法绕过 u 连到 u 的祖先，因而会成为
 *   一个大小为 subSize[v] 的独立连通块。
 *
 *   DFS 时依次累加这些独立子树的大小。新独立块与此前独立块之间贡献
 *   2 * subSize[v] * sum 个有序点对；遍历结束后，其余节点构成一个大小为
 *   n-1-sum 的连通块，再计算它与所有独立块之间的贡献。
 *
 * 复杂度分析：
 *   每个节点和每条边只在 Tarjan DFS 中访问常数次：
 *   - 时间复杂度：O(n+m)
 *   - 空间复杂度：O(n+m)
 */

#include <iostream>
#include <vector>
using namespace std;

// n、m 分别为节点数和无向边数。
int n, m;
// graph[u] 存储 u 的相邻节点。
vector<vector<int>> graph;
// dfn[u] 为 u 的访问时间；low[u] 为 u 的子树能回到的最小时间戳。
// subSize[u] 为以 u 为根的 DFS 子树大小。
vector<int> dfn, low, subSize;
// res[u] 为删除 u 的所有关联边后，不连通有序点对的数量。
vector<long long> res;
// Tarjan DFS 使用的递增时间戳。
int timer = 1;

// 从 u 开始 Tarjan DFS；p 是 u 在 DFS 树中的父节点。
void dfs(int u, int p) {
    // 首次访问 u，并初始化它的子树大小。
    dfn[u] = low[u] = timer++;
    subSize[u] = 1;
    // u 被孤立后，与其余 n-1 个节点产生双向的有序不连通点对。
    res[u] += 2 * (n - 1);

    // sum 是已确认会在删除 u 后独立出来的 DFS 子树总大小。
    long long sum = 0;
    for (int v: graph[u]) {
        // 无向边在邻接表中出现两次，跳过 DFS 树父边。
        // 题目保证没有重边，因此按父节点判断即可。
        if (v == p) {
            continue;
        }
        if (dfn[v] == 0) {
            // v 尚未访问，u-v 是一条 DFS 树边。
            dfs(v, u);
            // 汇总 v 子树的信息。
            low[u] = min(low[u], low[v]);
            subSize[u] += subSize[v];

            // v 的子树不能绕过 u 到达 u 的祖先，删去 u 的关联边后它自成一块。
            if (low[v] >= dfn[u]) {
                // 新独立块与此前所有独立块之间的有序不连通点对。
                res[u] += 2 * subSize[v] * sum;
                sum += subSize[v];
            }
        } else {
            // v 是已经访问的祖先，利用返祖边更新 u 的 low 值。
            low[u] = min(low[u], dfn[v]);
        }
    }
    // 剩余节点彼此仍连通，计算它们与所有独立块之间的有序不连通点对。
    res[u] += 2 * sum * (n - 1 - sum);
}

int main() {
    cin >> n >> m;
    // 每条道路为无向边，因此两个端点都加入邻接表。
    graph.assign(n + 1, {});
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    dfn.assign(n + 1, 0);
    low.assign(n + 1, 0);
    subSize.assign(n + 1, 0);
    res.assign(n + 1, 0);
    // 题目保证原图连通，从节点 1 出发即可访问所有节点。
    dfs(1, 0);

    // 依次输出删除每个节点关联边后的不连通有序点对数。
    for (int i = 1; i <= n; ++i) {
        cout << res[i] << '\n';
    }
    return 0;
}
