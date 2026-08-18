/**
 * Luogu P3128 - [USACO15DEC] Max Flow P
 *
 * 问题描述：
 *   n 个牛棚通过 n-1 条管道连成一棵树。给定 k 对端点，每对端点之间沿树上的唯一
 *   路径输送单位流量，路径两端和中间节点都会被计入。求经过同一节点的最大流量。
 *
 * 解法：倍增 LCA + 树上节点差分
 *   先以 1 号节点为根，通过 dfs 计算每个节点的深度和倍增祖先表 parent，其中
 *   parent[u][i] 表示 u 向上走 2^i 层后的祖先。这样可以在 O(log n) 时间内求出
 *   每条路径两个端点 u、v 的最近公共祖先 w。
 *
 *   对路径 u-v 进行节点差分：
 *       val[u]++
 *       val[v]++
 *       val[w]--
 *       val[parent[w][0]]--
 *   从叶节点向根累加后，u、v 两侧的贡献都会到达 w：w 原本收到 2，再减去自身的
 *   1 后保留 1；传到 w 的父节点时再被减去 1，路径影响便终止在 w。因此最终累加值
 *   val[x] 就是经过节点 x 的路径数量。
 *
 *   根节点 1 的父节点设为虚拟的 0 号节点。当 w=1 时，最后一次差分会写入 val[0]；
 *   0 号节点不参与树上累加和答案统计，所以无需对根节点额外分类讨论。
 *
 * 复杂度分析：
 *   - 建树：O(n)
 *   - 预处理倍增表：O(n log n)
 *   - 每条路径求 LCA 并差分：O(log n)
 *   - 回收全部差分：O(n)
 *   - 总时间复杂度：O((n+k) log n)
 *   - 空间复杂度：O(n log n)，递归栈最深可达 O(n)
 */

#include <iostream>
#include <vector>
using namespace std;

// graph 保存无向树；parent[u][i] 是节点 u 的 2^i 级祖先。
vector<vector<int>> graph, parent;
// val 是节点差分数组；depth[u] 是节点 u 到根节点 1 的距离。
vector<int> val, depth;
// solve 使用 visited 避免沿无向边返回已经处理的父节点。
vector<bool> visited;
// res 保存所有节点最终流量的最大值。
int res = 0;

// 以 1 为根建立父子关系，并预处理节点深度和倍增祖先表。
void dfs(int node, int d) {
    depth[node] = d;
    for (int child: graph[node]) {
        // parent[node][0] 是 node 的直接父节点，只递归处理其余相邻节点。
        if (parent[node][0] != child) {
            // 2^0=1，因此 child 向上一层就是当前节点 node。
            parent[child][0] = node;
            for (int i = 1; i < 20; i++) {
                // 两次向上 2^(i-1) 层，得到向上 2^i 层的祖先。
                parent[child][i] = parent[parent[child][i - 1]][i - 1];
            }
            // 儿子的深度比当前节点多 1。
            dfs(child, d + 1);
        }
    }
}

// 使用倍增表查询节点 u、v 的最近公共祖先。
int query(int u, int v) {
    if (depth[u] < depth[v]) {
        // 保证 u 的深度不小于 v，之后只需要先提升 u。
        swap(u, v);
    }
    int diff = depth[u] - depth[v];
    for (int i = 0; i < 20; i++) {
        // 深度差的第 i 个二进制位为 1，就让 u 向上跳 2^i 层。
        if (diff & (1 << i)) {
            u = parent[u][i];
        }
    }
    if (u == v) {
        // 深度对齐后重合，较浅节点本身就是 LCA。
        return u;
    }
    for (int i = 19; i >= 0; i--) {
        // 两个 2^i 级祖先不同，说明此次跳跃仍会停在 LCA 下方，可以同时上跳。
        if (parent[u][i] != parent[v][i]) {
            u = parent[u][i];
            v = parent[v][i];
        }
    }
    // 此时 u、v 位于 LCA 下方的两个分支，其共同的直接父节点就是 LCA。
    return parent[u][0];
}

// 从叶节点向根回收差分，返回 node 整棵子树累加后的流量。
int solve(int node) {
    visited[node] = true;
    if (node != 1 && graph[node].size() == 1) {
        // 非根叶节点没有需要合并的儿子，其最终流量就是当前差分值。
        res = max(res, val[node]);
        return val[node];
    }
    // 先保留 node 自己的差分值，再累加每棵儿子子树向上传递的贡献。
    int sum = val[node];
    for (int child: graph[node]) {
        if (!visited[child]) {
            sum += solve(child);
        }
    }
    // sum 是所有路径差分回收后经过 node 的路径数，用它更新全局最大值。
    res = max(res, sum);
    return sum;
}

int main() {
    // 输入规模较大，关闭同步并解除绑定以加快输入输出。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    // 使用邻接表保存 n-1 条无向边，节点编号范围为 [1,n]。
    graph.resize(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // parent 和 val 都包含 0 号位置；默认的 0 值使 0 成为根上方的虚拟祖先。
    parent.resize(n + 1, vector(20, 0));
    depth.resize(n + 1);
    val.resize(n + 1);
    // 以 1 号节点为根，预处理每个节点的深度和倍增祖先。
    dfs(1, 0);

    for (int i = 0; i < k; i++) {
        int u, v;
        cin >> u >> v;

        // 对整条节点路径 u-v 进行四点差分。
        int coParent = query(u, v);
        val[u]++;
        val[v]++;
        val[coParent]--;
        // coParent=1 时，这里修改 val[0]；虚拟节点 0 不会进入答案。
        val[parent[coParent][0]]--;
    }

    // 自底向上回收全部差分，并在回收过程中计算最大节点流量。
    visited.resize(n + 1, false);
    solve(1);
    // res 是任意一个真实牛棚所承受的最大流量。
    cout << res << endl;
    return 0;
}
