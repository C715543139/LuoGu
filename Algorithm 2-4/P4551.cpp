/**
 * Luogu P4551 - 最长异或路径
 *
 * 问题描述：
 *   给定一棵带权树，求任意两个节点之间路径上的边权异或值的最大值。
 *
 * 解法：根路径异或 + 哈希集合逐位贪心
 *   任取节点 1 为根，dist[u] 表示从根节点到节点 u 的路径异或值。
 *   对于任意两个节点 u、v，它们公共路径上的边在 dist[u] 和 dist[v] 中各出现一次，
 *   异或后相互抵消，因此 u 到 v 的路径异或值等于 dist[u] ^ dist[v]。
 *   原问题由此转化为：从所有 dist 中选出两个数，使它们的异或值最大。
 *
 *   从第 30 位到第 0 位依次确定答案。每轮只保留所有 dist 的当前位及更高位，
 *   并尝试令答案当前位为 1。若存在两个前缀 x、y 满足 x ^ y == tryRes，
 *   则 y == x ^ tryRes；可用哈希集合查询该 y 是否存在，从而判断 tryRes 是否可行。
 *   高位对数值的贡献大于所有低位之和，所以可以从高位到低位贪心确定答案。
 *
 * 复杂度分析：
 *   - 平均时间复杂度：O(31n) = O(n)
 *   - 空间复杂度：O(n)，递归 DFS 的调用栈最深为 O(n)
 *
 * 代码注意：
 *   当前读边循环执行 n 次，但含 n 个节点的树只有 n - 1 条边；该边界需要修正后才能提交。
 *   当树退化成长度为 n 的链时，递归 DFS 还可能因调用层数过深导致栈溢出。
 */

#include <iostream>
#include <vector>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;


// tree[u] 中的每个 pair 保存与 u 相邻的节点编号和对应边权。
vector<vector<pair<int, int>>> tree;
// dist[u]：根节点 1 到节点 u 的路径上所有边权的异或值。
vector<int> dist;
// 无向边会同时存储两个方向，用 visited 防止 DFS 在父子节点之间反复访问。
vector<bool> visited;

// val 表示根节点到当前节点 cur 的路径异或值。
void dfs(int cur, int val) {
    // 空邻接表对应单节点树；已访问节点通常是沿无向边返回的父节点。
    if (tree[cur].empty() || visited[cur]) {
        return;
    }

    visited[cur] = true;
    for (const auto [child, w]: tree[cur]) {
        // 根到 child 的路径等于根到 cur 的路径再连接当前权值为 w 的边。
        // 若 child 已访问（通常为父节点），下一次 dfs 会立即返回。
        dist[child] = val ^ w;
        dfs(child, dist[child]);
    }
}

int main() {
    int n;
    cin >> n;

    tree = vector<vector<pair<int, int>>>(n + 1);
    dist = vector(n + 1, 0);
    visited = vector(n + 1, false);

    for (int i = 0; i < n - 1; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        // 题目中的边是无向边，因此将两个方向都存入邻接表。
        tree[u].emplace_back(v, w);
        tree[v].emplace_back(u, w);
    }

    // 计算以节点 1 为根时，每个节点对应的根路径异或值。
    dfs(1, 0);

    // res 保存目前已确定的最大异或值高位；mask 指示当前需要保留的高位范围。
    int res = 0, mask = 0;
    for (int i = 30; i >= 0; i--) {
        // 将第 i 位加入掩码，此时 mask 覆盖第 30 位到第 i 位。
        mask |= 1 << i;

        // 保存所有 dist 在当前 mask 下的不同高位前缀。
        __gnu_pbds::gp_hash_table<int, __gnu_pbds::null_type> set;
        for (const int dis: dist) {
            set.insert(dis & mask);
        }

        // 已确定的更高位保持不变，并尝试令答案第 i 位为 1。
        int tryRes = res | 1 << i;
        for (const auto dis: set) {
            // 若另一个前缀 dis ^ tryRes 存在，两者异或后恰好得到 tryRes。
            if (set.find(dis ^ tryRes) != set.end()) {
                // 当前位可以取 1，保留这次尝试并继续确定下一位。
                res = tryRes;
                break;
            }
        }
    }
    cout << res << endl;
    return 0;
}
