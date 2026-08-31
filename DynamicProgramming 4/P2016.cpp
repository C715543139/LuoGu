/**
 * Luogu P2016 - [SEERC 2000] 战略游戏
 *
 * 问题描述：
 *   给定一棵树，需要在尽可能少的节点上放置士兵，使每条边都能被瞭望到。
 *   一个节点上的士兵可以瞭望与该节点相连的所有边。
 *
 * 解法：树形动态规划
 *   一条边只要它的任意一个端点放置了士兵，就能够被瞭望。
 *   因此问题等价于：选择尽可能少的节点，使树中的每条边至少有一个端点被选择，
 *   也就是求这棵树的最小点覆盖。
 *
 *   以节点 0 为根。设 solve(node, parent) 返回一个二元组：
 *     - first：node 放置士兵时，以 node 为根的子树所需的最少士兵数；
 *     - second：node 不放置士兵时，以 node 为根的子树所需的最少士兵数。
 *
 *   如果 node 放置士兵，那么它的每个子节点可以放置士兵，也可以不放置，
 *   因为 node 已经能够覆盖与子节点相连的边，所以选择子树中的较优状态：
 *     first = 1 + sum(min(childFirst, childSecond))。
 *
 *   如果 node 不放置士兵，那么为了覆盖 node 与每个子节点之间的边，
 *   每个子节点都必须放置士兵：
 *     second = sum(childFirst)。
 *
 *   叶子节点放置士兵需要 1 名，不放置士兵需要 0 名。
 *   使用记忆化搜索保存每个节点的计算结果，避免重复递归。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n)，每个节点和每条边只处理一次；
 *   - 空间复杂度：O(n)，用于存储树结构和 DP 结果。
 */

#include <iostream>
#include <vector>
using namespace std;

// te[i] 保存与第 i 个节点相邻的所有节点
vector<vector<int>> te;

// mp[i] 保存节点 i 在“放置士兵”和“不放置士兵”两种状态下的最优值
vector<pair<int, int>> mp;

// 计算以 node 为根、parent 为父节点的子树的两种状态
pair<int, int> solve(int node, int parent) {
    // 非根叶子节点没有子节点
    // 放置士兵可以覆盖与父节点相连的边，不放置则由父节点负责覆盖
    if (te[node].size() == 1 && node != 0) {
        return {1, 0};
    }

    // 如果当前节点已经计算过，直接返回记忆化结果
    if (mp[node] != pair(-1, -1)) {
        return mp[node];
    }

    // ari：当前节点放置士兵时的最少士兵数
    // nashi：当前节点不放置士兵时的最少士兵数
    int ari = 1, nashi = 0;

    // 枚举当前节点的所有相邻节点
    for (int v: te[node]) {
        // 跳过父节点，避免沿原路返回
        if (v == parent) {
            continue;
        }

        // 计算子节点对应子树的两种状态
        auto [childA, childN] = solve(v, node);

        // 当前节点放置士兵，子节点可以放置，也可以不放置
        ari += min(childA, childN);

        // 当前节点不放置士兵，为覆盖当前节点与子节点之间的边，子节点必须放置
        nashi += childA;
    }

    // 保存当前节点的两种状态并返回
    return mp[node] = {ari, nashi};
}

int main() {
    // 读入树的节点数
    int n;
    cin >> n;

    // 初始化邻接表和记忆化数组
    te.assign(n, {});
    mp.assign(n, {-1, -1});

    // 读入每个节点的编号和相邻节点
    for (int i = 0; i < n; i++) {
        int idx, k;
        cin >> idx >> k;

        // 输入中的每条边只出现一次，因此在这里补充无向边的另一方向
        for (int j = 0; j < k; j++) {
            int v;
            cin >> v;
            te[idx].push_back(v);
            te[v].push_back(idx);
        }
    }

    // 以节点 0 为根，取根节点放置和不放置士兵两种情况的较小值
    auto [ari, nashi] = solve(0, -1);
    cout << min(ari, nashi) << endl;
    return 0;
}
