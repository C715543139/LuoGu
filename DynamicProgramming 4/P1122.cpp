/**
 * Luogu P1122 - 最大子树和
 *
 * 问题描述：
 *   给定一棵带权树，每个节点有一个美丽指数。
 *   通过剪枝并丢弃其中一部分，最后保留的节点必须构成一个连通子树，
 *   求所有连通子树中节点权值和的最大值。
 *
 * 解法：树形动态规划
 *   以任意节点为根进行 DFS。对于以 node 为根的子树，
 *   可以考虑当前节点是否属于最终保留的连通子树。
 *
 *   当前代码使用两个状态：
 *     - include：保留 node 时，当前子树能够获得的最大权值；
 *     - exclude：不保留 node 时，当前子树能够获得的最大权值。
 *
 *   当 node 被保留时，如果某个子节点方向的贡献为负，可以直接剪掉该方向；
 *   如果贡献为正，则保留该方向能够增加答案。
 *   当 node 不被保留时，子树中的最优结果可以来自子节点的 include 或 exclude 状态。
 *
 *   树中的每条边都只需要遍历一次，因此树形 DP 的时间复杂度应为 O(n)，
 *   空间复杂度为 O(n)。
 *
 * 注意：
 *   求整棵树中的最大连通子树时，最终答案不一定包含指定的根节点，
 *   因此通常还需要在 DFS 过程中维护所有节点的最大值。
 */

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

// val[i] 表示第 i 朵花的美丽指数
vector<int> val;

// te[i] 保存与第 i 朵花直接相连的花朵编号
vector<vector<int>> te;

// mp[node] 保存节点 node 的记忆化搜索结果
unordered_map<int, pair<int, int>> mp;

// 计算以 node 为当前节点、parent 为其父节点时的两种状态
pair<int, int> solve(int node, int parent) {
    // 如果当前节点已经计算过，直接返回保存的结果
    if (mp.find(node) != mp.end()) {
        return mp[node];
    }

    // include：保留当前节点时的最大权值
    // exclude：不保留当前节点时的最大权值
    int include = val[node], exclude = -2147483647;

    // 枚举当前节点的所有相邻节点
    for (int v: te[node]) {
        // 跳过父节点，避免沿原路返回
        if (v == parent) {
            continue;
        }

        // 计算子节点对应子树的两种状态
        auto [childIn, childEx] = solve(v, node);

        // 保留 node 时，只在子树贡献为正时保留该子树
        include = max(include, include + childIn);

        // 不保留 node 时，取子树内部两种状态中的较大值
        exclude = max(exclude, max(childIn , childEx));
    }

    // 返回当前节点对应的两种状态
    return {include, exclude};
}

int main() {
    // 读入花朵数量
    int n;
    cin >> n;

    // 初始化美丽指数和树结构
    val.assign(n + 1, 0);
    te.assign(n + 1, {});

    // 读入每朵花的美丽指数
    for (int i = 1; i <= n; i++) {
        cin >> val[i];
    }

    // 读入树的 n-1 条枝干
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;

        // 枝干是无向边，需要加入两个端点的邻接表
        te[u].push_back(v);
        te[v].push_back(u);
    }

    // 从节点 1 开始计算树形 DP
    auto [include, exclude] = solve(1, 0);

    // 输出当前计算结果中的较大值
    cout << max(include, exclude) << endl;
    return 0;
}
