/**
 * Luogu P3388 - 【模板】割点（割顶）
 *
 * 问题描述：
 *   给定一张可能不连通的无向图，求所有割点。删除一个割点及其关联边后，原图的
 *   连通分量数量会增加。
 *
 * 解法：Tarjan 求割点
 *   dfn[u] 记录节点 u 第一次被访问的时间；low[u] 记录从 u 的 DFS 子树出发，
 *   经过树边和至多一条返祖边能够到达的最早节点的访问时间。
 *
 *   对于 DFS 树中的一条树边 u-v：
 *   - 若 u 不是根节点且 low[v]>=dfn[u]，v 的子树无法绕过 u 到达 u 的祖先，
 *     删除 u 后该子树会与图中其余部分分离，因此 u 是割点；
 *   - 若 u 是根节点，只有当 u 至少拥有两个 DFS 子树时，删除 u 才会使这些子树
 *     彼此分离，因此 u 是割点。
 *
 *   图可能不连通，所以从每个尚未访问的节点开始一次 Tarjan DFS。set 用于保存
 *   割点，使同一节点只出现一次，并自动按照节点编号升序排列。
 *
 * 复杂度分析：
 *   每个节点访问一次，每条无向边从两个方向各检查一次；set 插入至多发生 O(n)
 *   次，每次耗时 O(log n)：
 *   - 时间复杂度：O(n+m+n log n)
 *   - 空间复杂度：O(n+m)
 */

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

// graph[u] 中的每个元素为 {相邻节点, 原无向边编号}。
vector<vector<pair<int, int>>> graph;
// dfn[u] 为 u 的访问时间；low[u] 为 u 的 DFS 子树能够到达的最小时间戳。
vector<int> dfn, low;
// set 自动去重并按节点编号升序保存所有割点。
set<int> cut;
// Tarjan DFS 使用的递增时间戳。
int timer = 1;

// 从 u 开始寻找割点；pid 是进入 u 时经过的边编号，-1 表示 u 是 DFS 根节点。
void dfs(int u, int pid) {
    // 主函数会尝试从每个节点开始 DFS，已经访问过的节点无需再次处理。
    if (dfn[u] != 0) {
        return;
    }

    // subtree 只统计 u 的 DFS 树子节点数量，用于判断根节点是否为割点。
    int subtree = 0;
    // 首次访问 u 时，当前能够到达的最早节点就是 u 自身。
    dfn[u] = low[u] = timer++;
    for (auto [v, id]: graph[u]) {
        // 同一条无向边会在邻接表中出现两次，跳过进入 u 时经过的父边。
        // 使用边编号判断可以正确区分同一对节点之间的多条边。
        if (id == pid) {
            continue;
        }
        if (dfn[v] == 0) {
            // v 尚未访问，u-v 是 DFS 树边，v 是 u 的一个 DFS 子节点。
            subtree++;
            dfs(v, id);
            // v 的子树能够到达的最早节点也可以用于更新 u。
            low[u] = min(low[u], low[v]);

            // 非根节点 u 分隔了 v 的子树与 u 的祖先，因此 u 是割点。
            if (low[v] >= dfn[u] && pid != -1) {
                cut.insert(u);
            }
        } else {
            // v 已经访问，利用返祖边 u-v 更新 u 能够到达的最小时间戳。
            low[u] = min(low[u], dfn[v]);
        }
    }

    // DFS 根节点没有父节点；它至少拥有两个独立 DFS 子树时才是割点。
    if (pid == -1 && subtree > 1) {
        cut.insert(u);
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    graph.resize(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        // 无向边从两个方向存储，并共用同一个编号 i。
        graph[u].emplace_back(v, i);
        graph[v].emplace_back(u, i);
    }

    dfn.resize(n + 1, 0);
    low.resize(n + 1, 0);
    // 图不一定连通；已访问节点会在 dfs 开头直接返回，每个未访问节点会成为
    // 所在连通分量的 DFS 根节点。
    for (int i = 1; i <= n; ++i) {
        dfs(i, -1);
    }
    // set 的大小就是不同割点的数量，遍历顺序天然为节点编号升序。
    cout << cut.size() << '\n';
    for (int u: cut) {
        cout << u << ' ';
    }
    return 0;
}
