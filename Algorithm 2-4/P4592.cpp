/**
 * Luogu P4592 - [TJOI2018] 异或
 *
 * 问题描述：
 *   给定一棵以节点 1 为根的树，每个节点带有一个权值。需要回答两类静态查询：
 *   1. 在节点 x 的子树中选择节点 u，使 W[u] ^ z 最大；
 *   2. 在节点 x 到节点 y 的简单路径上选择节点 u，使 W[u] ^ z 最大。
 *
 * 代码保留两种解法：
 *   - solve1：递归遍历子树、沿父节点枚举路径，单次查询最坏为 O(n)，
 *     保留作为直观实现和随机对拍基准；
 *   - solve2：迭代 DFS + 倍增 LCA + 可持久化 01 Trie，main 实际调用此版本。
 *
 * solve2 的核心结构：
 *   1. 迭代 DFS 求 parent、depth、DFS 序 tin/tout，并建立根路径 Trie 版本；
 *   2. subtreeRoot[i] 保存 DFS 序前 i 个节点的权值；由于一棵子树在 DFS 序中
 *      对应连续区间 [tin[x], tout[x]]，两个版本相减即可得到子树内的权值集合；
 *   3. pathRoot[u] 保存根节点 1 到 u 路径上的全部点权。令 lca 为 x、y 的最近
 *      公共祖先，则 x 到 y 路径上的权值集合可由四个版本的计数表示：
 *
 *        pathRoot[x] + pathRoot[y]
 *        - pathRoot[lca] - pathRoot[parent[lca]]。
 *
 *      该式使 lca 恰好保留一次，lca 上方的公共祖先全部抵消。
 *
 *   可持久化 01 Trie 的每个节点记录对应二进制前缀出现的次数。查询时从最高位
 *   向最低位处理，优先选择与 z 当前位相反且区间计数大于 0 的分支，使当前异或位
 *   得到 1；高位确定后再处理低位。
 *
 * 复杂度分析：
 *   设权值位数 B = 30。
 *   - solve1：单次查询最坏 O(n)，递归栈最深 O(n)；
 *   - solve2：预处理 O(n(B + log n))，每次子树查询 O(B)，
 *     每次路径查询 O(B + log n)，空间复杂度 O(n(B + log n))。
 */

#include <algorithm>
#include <array>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

void solve1();

/**
 * solve1 使用的普通树结构。
 *
 * 它先从根节点 1 递归建立 parent 和 depth。子树查询递归访问全部后代；
 * 路径查询先让两个端点到达相同深度，再同步向父节点移动到最近公共祖先。
 */
class Tree {
    vector<int> W, parent, depth;
    vector<vector<int>> graph;
    vector<bool> visited;

public:
    Tree(int n)
        : W(n + 1), parent(n + 1), depth(n + 1),
          graph(n + 1), visited(n + 1, false) {}

    // 保存节点 i 的权值。
    void setW(int i, int w) {
        W[i] = w;
    }

    // 输入边没有方向，因此将两个方向都加入邻接表。
    void addEdge(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // 从根开始确定每个节点的父节点和深度。
    void dfsFn(int cur, int curDepth) {
        if (visited[cur]) {
            return;
        }
        visited[cur] = true;
        depth[cur] = curDepth;

        for (int child: graph[cur]) {
            if (!visited[child]) {
                parent[child] = cur;
                dfsFn(child, curDepth + 1);
            }
        }
    }

    void dfs() {
        dfsFn(1, 1);
    }

    // 递归枚举 x 的全部后代，直接计算子树查询答案。
    int findSubTree(int x, int z) {
        int res = W[x] ^ z;
        for (int node: graph[x]) {
            // parent[node] == x 表示 node 是 x 的孩子，避免沿无向边返回父节点。
            if (parent[node] == x) {
                res = max(res, findSubTree(node, z));
            }
        }
        return res;
    }

    // 沿父指针枚举 x 到 y 的简单路径，并在移动过程中更新答案。
    int findPath(int x, int y, int z) {
        int xDepth = depth[x], yDepth = depth[y];
        int res = max(W[x] ^ z, W[y] ^ z);

        // 先让较深的端点上升，使两个端点处于相同深度。
        while (xDepth > yDepth) {
            x = parent[x];
            xDepth = depth[x];
            res = max(res, W[x] ^ z);
        }
        while (xDepth < yDepth) {
            y = parent[y];
            yDepth = depth[y];
            res = max(res, W[y] ^ z);
        }

        // 同步上升，直至 x == y；相遇节点就是最近公共祖先。
        while (x != y) {
            x = parent[x];
            y = parent[y];
            res = max(res, max(W[x] ^ z, W[y] ^ z));
        }
        return res;
    }
};

// 暴力版本：保留用于理解查询含义及与 solve2 对拍。
void solve1() {
    int n, q;
    cin >> n >> q;

    Tree tree(n);
    for (int i = 1; i <= n; i++) {
        int w;
        cin >> w;
        tree.setW(i, w);
    }
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        tree.addEdge(u, v);
    }
    tree.dfs();

    for (int i = 0; i < q; i++) {
        int op;
        cin >> op;
        if (op == 1) {
            int x, z;
            cin >> x >> z;
            cout << tree.findSubTree(x, z) << '\n';
        } else {
            int x, y, z;
            cin >> x >> y >> z;
            cout << tree.findPath(x, y, z) << '\n';
        }
    }
}

void solve2();

// W[i], z < 2^30，因此处理第 29 位到第 0 位。
constexpr int MAX_BIT = 29;

// n <= 10^5，2^17 已经大于最大节点数。
constexpr int MAX_LOG = 17;
constexpr int MAX_VERTEX_COUNT = 100000;

// solve2 为根路径和 DFS 序各插入 n 个权值；每次插入产生 1 + 30 个新节点。
constexpr int MAX_TRIE_NODE_COUNT =
        2 * MAX_VERTEX_COUNT * (MAX_BIT + 2) + 5;

/**
 * 可持久化 01 Trie。
 *
 * 每个版本都是一个只读根节点编号。插入 value 时，仅复制根到叶节点的路径，
 * 其他分支继续与旧版本共享。count 表示当前版本中有多少个数经过对应节点。
 */
class PersistentBinaryTrie {
    struct Node {
        // child[0]、child[1] 分别指向下一位为 0、1 的节点编号。
        int child[2];

        // 当前二进制前缀在该版本中出现的次数，重复权值会被分别计数。
        int count;
    };

    // 连续节点池能避免数百万次独立的动态内存分配。
    Node nodes[MAX_TRIE_NODE_COUNT];
    int nodeCount;

    // 复制旧节点并返回新节点编号，旧版本本身保持不变。
    int cloneNode(int source) {
        int target = ++nodeCount;
        nodes[target] = nodes[source];
        return target;
    }

public:
    PersistentBinaryTrie() : nodeCount(0) {
        // 0 号节点表示空 Trie，也是所有尚不存在分支的统一空节点。
        nodes[0].child[0] = nodes[0].child[1] = 0;
        nodes[0].count = 0;
    }

    /**
     * 在 previousRoot 版本中插入 value，返回新版本的根节点编号。
     * 一次插入复制根节点及 30 个二进制位节点。
     */
    int insert(int previousRoot, int value) {
        int newRoot = cloneNode(previousRoot);
        nodes[newRoot].count++;

        int previous = previousRoot;
        int current = newRoot;

        for (int bit = MAX_BIT; bit >= 0; bit--) {
            int digit = value >> bit & 1;
            int previousChild = nodes[previous].child[digit];
            int newChild = cloneNode(previousChild);

            nodes[newChild].count++;
            nodes[current].child[digit] = newChild;

            previous = previousChild;
            current = newChild;
        }

        return newRoot;
    }

    /**
     * 在 rightRoot - leftRoot 表示的权值区间中，求与 value 的最大异或值。
     * 用于 DFS 序上的子树区间查询。
     */
    int queryRange(int rightRoot, int leftRoot, int value) const {
        int right = rightRoot;
        int left = leftRoot;
        int result = 0;

        for (int bit = MAX_BIT; bit >= 0; bit--) {
            int digit = value >> bit & 1;
            int wanted = digit ^ 1;

            int rightWanted = nodes[right].child[wanted];
            int leftWanted = nodes[left].child[wanted];

            // 两个版本的计数差大于 0，说明查询区间中存在期望分支。
            if (nodes[rightWanted].count - nodes[leftWanted].count > 0) {
                result |= 1 << bit;
                right = rightWanted;
                left = leftWanted;
            } else {
                right = nodes[right].child[digit];
                left = nodes[left].child[digit];
            }
        }

        return result;
    }

    /**
     * 使用四个根路径版本表示 x 到 y 的节点权值集合，求与 value 的最大异或值。
     * 集合计数为 rootX + rootY - rootLca - rootParentLca。
     */
    int queryPath(int rootX, int rootY, int rootLca,
                  int rootParentLca, int value) const {
        int x = rootX;
        int y = rootY;
        int lca = rootLca;
        int parentLca = rootParentLca;
        int result = 0;

        for (int bit = MAX_BIT; bit >= 0; bit--) {
            int digit = value >> bit & 1;
            int wanted = digit ^ 1;

            int xWanted = nodes[x].child[wanted];
            int yWanted = nodes[y].child[wanted];
            int lcaWanted = nodes[lca].child[wanted];
            int parentLcaWanted = nodes[parentLca].child[wanted];

            // 四个版本在期望分支上的计数之和，就是路径中落入该分支的节点数。
            int available = nodes[xWanted].count + nodes[yWanted].count
                            - nodes[lcaWanted].count
                            - nodes[parentLcaWanted].count;

            if (available > 0) {
                result |= 1 << bit;
                x = xWanted;
                y = yWanted;
                lca = lcaWanted;
                parentLca = parentLcaWanted;
            } else {
                x = nodes[x].child[digit];
                y = nodes[y].child[digit];
                lca = nodes[lca].child[digit];
                parentLca = nodes[parentLca].child[digit];
            }
        }

        return result;
    }
};

/**
 * 倍增求最近公共祖先。
 * up[u][k] 表示节点 u 向上移动 2^k 步后到达的祖先。
 */
int findLca(int x, int y, const vector<int> &depth,
            const vector<array<int, MAX_LOG + 1>> &up) {
    if (depth[x] < depth[y]) {
        swap(x, y);
    }

    // 先把较深节点提升到与另一个节点相同的深度。
    int difference = depth[x] - depth[y];
    for (int bit = 0; bit <= MAX_LOG; bit++) {
        if (difference >> bit & 1) {
            x = up[x][bit];
        }
    }

    if (x == y) {
        return x;
    }

    // 从大步长到小步长同步提升，保持两节点仍处于 LCA 的不同子树中。
    for (int bit = MAX_LOG; bit >= 0; bit--) {
        if (up[x][bit] != up[y][bit]) {
            x = up[x][bit];
            y = up[y][bit];
        }
    }

    return up[x][0];
}

// 高效版本：迭代 DFS、倍增 LCA 与两类可持久化 Trie 查询。
void solve2() {
    int n, q;
    cin >> n >> q;

    vector<int> W(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> W[i];
    }

    vector<vector<int>> graph(n + 1);
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // Trie 节点池约含 620 万个节点，放在静态存储区以免占用函数调用栈。
    static PersistentBinaryTrie trie;

    vector<int> parent(n + 1), depth(n + 1);
    vector<int> tin(n + 1), tout(n + 1), subtreeSize(n + 1, 1);
    vector<int> euler(n + 1), pathRoot(n + 1);
    vector<array<int, MAX_LOG + 1>> up(n + 1);
    vector<int> order;
    order.reserve(n);

    /**
     * 迭代 DFS 的栈替代递归调用栈。
     * 节点出栈时记录先序位置，并由父节点的 Trie 版本插入当前点权，建立 pathRoot。
     */
    stack<int> pending;
    pending.push(1);
    int timer = 0;

    while (!pending.empty()) {
        int cur = pending.top();
        pending.pop();

        tin[cur] = ++timer;
        euler[timer] = cur;
        order.push_back(cur);

        // pathRoot[cur] 保存根节点 1 到 cur 的全部点权。
        pathRoot[cur] = trie.insert(pathRoot[parent[cur]], W[cur]);

        up[cur][0] = parent[cur];
        for (int bit = 1; bit <= MAX_LOG; bit++) {
            up[cur][bit] = up[up[cur][bit - 1]][bit - 1];
        }

        // 逆序压栈可使迭代 DFS 的访问顺序接近常见递归 DFS 的邻接表顺序。
        for (auto it = graph[cur].rbegin(); it != graph[cur].rend(); ++it) {
            int child = *it;
            if (child == parent[cur]) {
                continue;
            }

            parent[child] = cur;
            depth[child] = depth[cur] + 1;
            pending.push(child);
        }
    }

    /**
     * 逆序累计子树大小。先序 DFS 中每棵子树占据连续区间，因此
     * tout[u] = tin[u] + subtreeSize[u] - 1。
     */
    for (int i = n - 1; i > 0; i--) {
        int cur = order[i];
        subtreeSize[parent[cur]] += subtreeSize[cur];
    }
    for (int u = 1; u <= n; u++) {
        tout[u] = tin[u] + subtreeSize[u] - 1;
    }

    /**
     * subtreeRoot[i] 保存 euler[1...i] 对应的点权。
     * subtreeRoot[r] - subtreeRoot[l - 1] 就表示 DFS 序区间 [l, r]。
     */
    vector<int> subtreeRoot(n + 1);
    for (int i = 1; i <= n; i++) {
        subtreeRoot[i] = trie.insert(subtreeRoot[i - 1], W[euler[i]]);
    }

    for (int i = 0; i < q; i++) {
        int op;
        cin >> op;
        if (op == 1) {
            int x, z;
            cin >> x >> z;

            // x 的子树对应 DFS 序上的连续区间 [tin[x], tout[x]]。
            cout << trie.queryRange(
                subtreeRoot[tout[x]],
                subtreeRoot[tin[x] - 1],
                z
            ) << '\n';
        } else {
            int x, y, z;
            cin >> x >> y >> z;

            int lca = findLca(x, y, depth, up);
            cout << trie.queryPath(
                pathRoot[x],
                pathRoot[y],
                pathRoot[lca],
                pathRoot[parent[lca]],
                z
            ) << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 使用满足完整数据范围的可持久化 01 Trie 实现。
    solve2();
    return 0;
}
