/**
 * Luogu P3379 - 【模板】最近公共祖先（LCA）
 *
 * 问题描述：
 *   给定一棵以 s 为根、包含 n 个节点的树。对于每次询问 u、v，求同时作为 u 和 v
 *   祖先且深度最大的节点，即它们的最近公共祖先。
 *
 * 解法：倍增 LCA
 *   parent[u][i] 表示节点 u 向上走 2^i 层后到达的祖先。已知 u 向上 2^(i-1) 层
 *   的祖先后，再从该祖先向上走 2^(i-1) 层，即可得到：
 *       parent[u][i] = parent[parent[u][i-1]][i-1]
 *   由于 n <= 5*10^5 且 2^19 > 5*10^5，保存 i=0..19 共 20 层祖先即可。
 *
 *   查询时先让较深节点按照深度差的二进制位向上跳，使两个节点深度相同。如果此时
 *   两节点重合，该节点就是 LCA；否则从大到小尝试让二者同时向上跳 2^i 层。只有
 *   两个 2^i 级祖先不同时才跳，以保证二者始终位于 LCA 下方的不同分支。循环结束
 *   后二者恰好位于 LCA 的两个不同儿子方向，其共同的直接父节点就是 LCA。
 *
 * 复杂度分析：
 *   - 建树：O(n)
 *   - 预处理倍增表：O(n log n)
 *   - 每次查询：O(log n)
 *   - 总时间复杂度：O((n+m) log n)
 *   - 空间复杂度：O(n log n)
 *
 * 代码说明：
 *   dfsRec 和 dfsIter 分别给出递归、分层迭代两种预处理方式，main 当前调用 dfsRec。
 *   链状树会使 dfsRec 的递归深度达到 O(n)，可能受到运行环境栈空间限制。
 */

#include <iostream>
#include <vector>
using namespace std;

// graph 保存无向树；parent[u][i] 是 u 的 2^i 级祖先。
vector<vector<int>> graph, parent;
// depth[u] 是 u 到题目指定根节点的边数。
vector<int> depth;
// 无向边会同时出现在两个节点的邻接表中，visited 用于避免返回父节点。
vector<bool> visited;

// 递归遍历整棵树，同时计算节点深度和倍增祖先表。
void dfsRec(int node, int d) {
    visited[node] = true;
    depth[node] = d;
    for (int child: graph[node]) {
        if (!visited[child]) {
            // 2^0=1，因此 child 向上一层的祖先就是当前节点 node。
            parent[child][0] = node;
            for (int i = 1; i < 20; i++) {
                // 两次向上 2^(i-1) 层，等价于一次向上 2^i 层。
                parent[child][i] = parent[parent[child][i - 1]][i - 1];
            }
            // 儿子的深度比当前节点多 1。
            dfsRec(child, d + 1);
        }
    }
}

// 与 dfsRec 等价的分层迭代预处理方式，避免使用递归调用栈。
void dfsIter(int root) {
    // cur 保存当前深度层，next 收集下一深度层。
    vector<int> cur, next;
    cur.push_back(root);
    visited[root] = true;
    int d = depth[root] = 0;
    while (!cur.empty()) {
        // cur 的所有未访问邻接点都位于下一层，深度统一增加 1。
        d++;
        for (int node: cur) {
            for (int child: graph[node]) {
                if (!visited[child]) {
                    visited[child] = true;
                    depth[child] = d;
                    // 发现 child 时，其父节点和父节点的倍增表都已经计算完成。
                    parent[child][0] = node;
                    for (int i = 1; i < 20; i++) {
                        parent[child][i] = parent[parent[child][i - 1]][i - 1];
                    }
                    next.push_back(child);
                }
            }
        }
        // 交换后 cur 成为下一轮处理的深度层，next 暂时保存旧层。
        cur.swap(next);
        next.clear();
    }
}

// 查询节点 u、v 的最近公共祖先。
int query(int u, int v) {
    if (depth[v] > depth[u]) {
        // 保证 u 的深度不小于 v，之后只需要先提升 u。
        swap(u, v);
    }
    int diff = depth[u] - depth[v];
    for (int i = 0; i < 20; i++) {
        // diff 的第 i 个二进制位为 1，就让 u 向上跳 2^i 层。
        if (diff & (1 << i)) {
            u = parent[u][i];
        }
    }
    if (u == v) {
        // 深度对齐后重合，说明较浅节点本身就是另一个节点的祖先。
        return u;
    }
    for (int i = 19; i >= 0; i--) {
        // 若 2^i 级祖先不同，这次跳跃仍不会到达或越过 LCA，可以同时上跳。
        if (parent[u][i] != parent[v][i]) {
            u = parent[u][i];
            v = parent[v][i];
        }
    }
    // 此时 u、v 位于 LCA 下方的两个分支，二者的直接父节点相同且就是 LCA。
    return parent[u][0];
}

int main() {
    // 输入规模可达 5*10^5，关闭同步并解除绑定以加快输入输出。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s;
    cin >> n >> m >> s;

    // 使用邻接表保存 n-1 条无向边，节点编号范围为 [1,n]。
    graph.resize(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // 每个节点保存 20 层倍增祖先，数组下标 i 对应向上 2^i 层。
    depth.resize(n + 1);
    parent.resize(n + 1, vector(20, 0));
    visited.resize(n + 1, false);
    for (int i = 0; i < 20; i++) {
        // 根节点再向上仍视为根节点，避免倍增时跳到无效的 0 号节点。
        parent[s][i] = s;
    }
    // 从题目指定根节点出发，预处理所有深度和倍增祖先。
    dfsRec(s, 0);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        // 每次查询独立使用预处理结果，单次只需检查 20 个倍增层。
        cout << query(u, v) << '\n';
    }
    return 0;
}
