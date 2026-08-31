/**
 * Luogu P2015 - 二叉苹果树
 *
 * 问题描述：
 *   给定一棵根节点为 1 的二叉树，每条边上有一定数量的苹果。
 *   需要保留 Q 条边，并且保留的边必须与根节点连通，求最多能够保留的苹果数量。
 *
 * 解法：树形动态规划
 *   如果一条边没有被保留，那么它下面的整棵子树都无法与根节点连通，
 *   因此只需要考虑从当前节点向下保留的边数和能够获得的苹果数量。
 *
 *   solve(node, parent, remain) 表示在以 node 为根、parent 为其父节点的子树中，
 *   恰好保留 remain 条边时能够获得的最大苹果数量。
 *   当前节点至多有两个子节点，记为 l 和 r。
 *
 *   如果只保留 node 到 l 的边，那么这条边占用 1 个名额，
 *   左子树还可以继续保留 remain-1 条边：
 *     solve(l, node, remain-1) + leftWeight。
 *
 *   如果只保留 node 到 r 的边，那么对应的转移为：
 *     solve(r, node, remain-1) + rightWeight。
 *
 *   如果两条子边都保留，那么两条边占用 2 个名额，
 *   剩余的 remain-2 条边需要在左右子树之间分配。
 *   枚举左子树保留 leftEdges 条边，右子树则保留 remain-2-leftEdges 条边：
 *     solve(l, node, leftEdges)
 *     + solve(r, node, remain-2-leftEdges)
 *     + leftWeight + rightWeight。
 *
 *   使用记忆化搜索保存相同 node 和 remain 状态的结果，避免重复计算。
 *   输入中的每个节点最多有两个儿子，因此可以通过排除 parent 找到当前节点的子节点。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(nQ^2)，其中合并左右子树时需要枚举边数分配；
 *   - 空间复杂度：O(nQ)，用于保存树结构和记忆化状态。
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

// te[i] 保存与第 i 个节点相连的节点及对应边上的苹果数量
vector<vector<pair<int, int>>> te;

// memo[node][remain] 保存已经计算过的 solve 状态
vector<vector<long long>> memo;

// 不可行状态的返回值，使用有符号类型才能表示负无穷
constexpr long long NEG_INF = numeric_limits<long long>::lowest() / 4;

// 尚未计算状态的标记值
constexpr long long UNVISITED = numeric_limits<long long>::max();

// 计算当前节点子树在指定剩余边数下能够获得的最大苹果数量
long long solve(int node, int parent, int remain) {
    // 边数不能为负，表示当前分配方案不可行
    if (remain < 0) {
        return NEG_INF;
    }

    // 不再保留任何边时，不能获得新的苹果
    if (remain == 0) {
        return 0;
    }

    // 当前节点没有子节点时，无法保留任何边
    if ((node != 1 && te[node].size() == 1) || (node == 1 && te[node].empty())) {
        return NEG_INF;
    }

    // 如果当前状态已经计算过，直接返回记忆化结果
    if (memo[node][remain] != UNVISITED) {
        return memo[node][remain];
    }

    // 排除父节点，得到当前节点的所有子节点及对应边权
    vector<pair<int, int>> children;
    for (auto [next, weight]: te[node]) {
        if (next != parent) {
            children.emplace_back(next, weight);
        }
    }

    // 当前节点没有子节点时，remain 条边无法在该子树中实现
    if (children.empty()) {
        return memo[node][remain] = NEG_INF;
    }

    long long res = NEG_INF;

    // 只保留一条子边：该边占用 1 条名额，其余名额交给对应子树
    for (auto [child, weight]: children) {
        res = max(res, solve(child, node, remain - 1) + weight);
    }

    // 题目保证非叶节点有两个子节点
    if (children.size() == 2 && remain >= 2) {
        auto [left, leftWeight] = children[0];
        auto [right, rightWeight] = children[1];

        // 同时保留两条子边，枚举剩余边数在左右子树之间的分配方式
        for (int leftEdges = 0; leftEdges <= remain - 2; ++leftEdges) {
            int rightEdges = remain - 2 - leftEdges;

            long long candidate = solve(left, node, leftEdges)
                                  + solve(right, node, rightEdges)
                                  + leftWeight + rightWeight;
            res = max(res, candidate);
        }
    }

    // 保存并返回当前状态的最优结果
    return memo[node][remain] = res;
}

int main() {
    // 读入节点数量和需要保留的边数
    int n, q;
    cin >> n >> q;

    // 初始化树的邻接表
    te.assign(n + 1, {});

    // 读入每条边：u、v 为端点，w 为该边上的苹果数量
    for (int i = 0; i < n - 1; ++i) {
        int u, v, w;
        cin >> u >> v >> w;

        // 无向边需要加入两个端点的邻接表
        te[u].emplace_back(v, w);
        te[v].emplace_back(u, w);
    }

    // 初始化记忆化数组，-1 不能作为未访问标记，因为它可能是合法的苹果总数
    memo.assign(n + 1, vector<long long>(q + 1, UNVISITED));

    // 从根节点 1 开始，恰好保留 q 条边
    cout << solve(1, 0, q) << endl;
    return 0;
}
