/**
 * Luogu P1955 - 程序自动分析
 *
 * 问题描述：
 *   给定 n 个形如 (i, j, e) 的约束条件：
 *     - e = 1 表示变量 xi 和 xj 必须相等
 *     - e = 0 表示变量 xi 和 xj 必须不等
 *   判断是否存在一种变量赋值方案，使得所有约束条件同时满足。
 *
 * 解法一 (solve1)：图论 BFS 可达性判定
 *   - 将相等关系构建为无向图
 *   - 对每个不等关系，从 i 出发 BFS，检查是否可达 j：
 *       若可达，则说明 i 和 j 被要求既相等又不相等，矛盾。
 *
 * 解法二 (solve2)：并查集（Disjoint Set Union, DSU）
 *   - 将所有相等约束的变量合并到同一集合
 *   - 遍历所有不等约束，若两个变量已在同一集合，则矛盾
 *   - 时间复杂度 O(n·α(n))，优于 BFS 方法
 */

#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
using namespace std;

/**
 * 广度优先搜索（BFS）—— 找出从 start 出发可达的所有节点
 *
 * @param graph     邻接表表示的无向图（相等关系图）
 * @param reachable 可达性记录表
 *                   reachable[start] 存储从 start 出发所有可达节点
 * @param start     BFS 的起始节点编号
 * @param n         约束条件总数，作为循环上限避免死循环
 *
 * 算法流程：
 *   1. 将 start 的所有直接邻居加入当前层队列 cur
 *   2. 遍历 cur，将未访问节点的邻居加入下一层 next
 *   3. 最多迭代 n+1 层（因为最多 n 条边）
 *   4. 所有可达节点都记录到 reachable[start] 中
 */
void bfs(unordered_map<int, set<int>> &graph, unordered_map<int, set<int>> &reachable, int start,
         int n) {
    // 当前层节点：初始为 start 的所有直接邻居
    vector<int> cur{graph[start].begin(), graph[start].end()};
    // 直接邻居也属于可达节点
    reachable[start].insert(cur.begin(), cur.end());
    // visited 避免重复访问
    unordered_map<int, bool> visited;
    visited[start] = true;  // 起始节点已访问

    // 最多迭代 n 次，按层遍历
    for (int i = 0; i < n && !cur.empty(); i++) {
        vector<int> next;  // 下一层节点
        for (int node : cur) {
            if (!visited[node]) {
                visited[node] = true;
                // 将当前节点的所有邻居加入下一层
                next.insert(next.end(), graph[node].begin(), graph[node].end());
                // 记录 start 到这些邻居的可达性
                reachable[start].insert(graph[node].begin(), graph[node].end());
            }
        }
        cur = next;  // 进入下一层
    }
}

/**
 * 解法一：基于 BFS 图论可达性判定
 *
 * 思路：
 *   相等关系构成无向图，不等关系作为验证条件。
 *   对每条不等关系 (i, j)：
 *     - 检查从 i 出发可否到达 j；
 *     - 若可达，则矛盾 → 输出 NO；
 *     - 否则继续检查。
 *
 * 注意：
 *   - 若 i == j 且 e == 0（自身要求不等于自身），直接矛盾
 *   - 不可在输入循环中提前 break，否则会导致输入残留（本题多组数据）
 */
void solve1() {
    int t;
    cin >> t;  // t 组测试数据
    while (t--) {
        int n;
        cin >> n;  // 每组的约束条件数量

        // graph: 存储相等关系构建的无向图
        unordered_map<int, set<int>> graph;
        // neEdge: 存储不等关系对
        set<pair<int, int>> neEdge;

        bool flag = true;  // 标记是否仍然可行
        for (int k = 0; k < n; k++) {
            int i, j, e;
            cin >> i >> j >> e;  // 输入：变量 i、变量 j、约束类型 e
            // 不要在此处跳出循环，否则会产生输入残留

            // 同一个变量：e=0 则矛盾
            if (i == j) {
                if (e == 0) {
                    flag = false;
                }
                continue;  // e=1 时自身等于自身，无需处理
            }

            if (e == 1) {
                // 相等约束：在 i 和 j 之间建立无向边
                graph[i].insert(j);
                graph[j].insert(i);
            } else {
                // 不等约束：记录到待验证集合
                neEdge.insert({i, j});
            }
        }

        // 若尚未发现矛盾，验证所有不等约束
        if (flag) {
            // reachable: 记录每个节点的可达节点集合（用于缓存 BFS 结果）
            unordered_map<int, set<int>> reachable;
            for (auto [fst, sec] : neEdge) {
                // 若 fst 尚未计算过可达集，进行 BFS
                if (reachable.find(fst) == reachable.end()) {
                    bfs(graph, reachable, fst, n + 1);
                }
                // 若 fst 可达 sec，说明两个变量既是相等又是不等 → 矛盾
                if (reachable[fst].find(sec) != reachable[fst].end()) {
                    flag = false;
                    break;
                }
            }
        }

        // 输出结果
        if (flag) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }
}

/**
 * 并查集（Disjoint Set Union, DSU）
 *
 * 用于快速判断两个元素是否属于同一集合，支持：
 *   - insert(v):  添加一个新的元素 v（初始化父节点为自身）
 *   - find(v):    查找 v 的根节点（含路径压缩优化）
 *   - same(v, w): 判断 v 和 w 是否在同一集合
 *   - unite(v, w): 合并 v 和 w 所在的集合
 */
class DSU {
    // parent: 存储每个元素父节点的哈希表
    // 若 parent[v] == v，则 v 是根节点
    unordered_map<int, int> parent;

public:
    /**
     * 插入新元素 v
     * 若 v 尚未存在，将其父节点设为自身（即作为独立集合的根）
     */
    void insert(int v) {
        if (parent[v] == 0) {
            parent[v] = v;  // 初始化为自指，表示独立的根
        }
    }

    /**
     * 查找元素 v 所在集合的根（带路径压缩）
     * 递归向上查找，同时将路径上所有节点直接连到根
     */
    int find(int v) {
        if (parent[v] != v) {
            parent[v] = find(parent[v]);  // 路径压缩
        }
        return parent[v];
    }

    /**
     * 判断 v 和 w 是否属于同一集合
     */
    bool same(int v, int w) {
        return find(v) == find(w);
    }

    /**
     * 合并 v 和 w 所在的集合
     * 将 v 的根连接到 w 的根上（简单合并，未使用按秩合并优化）
     */
    void unite(int v, int w) {
        int rootV = find(v);
        int rootW = find(w);

        if (rootV != rootW) {
            parent[rootV] = rootW;  // 将 rootV 的父节点设为 rootW
        }
    }
};

/**
 * 解法二：基于并查集（DSU）的判定
 *
 * 思路：
 *   1. 遍历所有约束条件：
 *      - e == 1（相等）：将两个变量合并到同一集合
 *      - e == 0（不等）：暂存到 neEdge，后续验证
 *   2. 遍历所有不等约束：
 *      - 若两个变量已在同一集合中，则矛盾 → 输出 NO
 *   3. 所有约束都通过验证 → 输出 YES
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n · α(n))，其中 α(n) 为反阿克曼函数，近似常数
 *   - 空间复杂度：O(m)，m 为不同变量的数量
 *   - 相比 solve1 的 BFS 方法效率更高
 */
void solve2() {
    int t;
    cin >> t;  // t 组测试数据
    while (t--) {
        int n;
        cin >> n;  // 每组的约束条件数量

        // neEdge: 存储不等关系对，稍后统一验证
        set<pair<int, int>> neEdge;
        DSU dsu;  // 并查集实例

        bool flag = true;  // 标记是否仍然可行
        for (int i = 0; i < n; i++) {
            int a, b, e;
            cin >> a >> b >> e;  // 输入：变量 a、变量 b、约束类型 e

            if (e == 0) {
                // 不等约束
                if (a == b) {
                    flag = false;  // 自身要求不等于自身 → 矛盾
                } else {
                    neEdge.insert({a, b});  // 暂存不等关系
                    dsu.insert(a);           // 确保变量在并查集中
                    dsu.insert(b);
                }
            } else {
                // 相等约束：将两个变量合并到同一集合
                dsu.insert(a);
                dsu.insert(b);
                dsu.unite(a, b);
            }
        }

        // 若尚未发现矛盾，检查所有不等约束
        if (flag) {
            for (auto [a, b] : neEdge) {
                // 若 a 和 b 已在同一集合中，说明它们必须相等
                // 但又要求不等 → 矛盾
                if (dsu.same(a, b)) {
                    flag = false;
                    break;
                }
            }
        }

        // 输出结果
        if (flag) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }
}

int main() {
    solve2();  // 使用并查集解法（效率更高）
    return 0;
}
