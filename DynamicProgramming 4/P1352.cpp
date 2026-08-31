/**
 * Luogu P1352 - 没有上司的舞会
 *
 * 问题描述：
 *   职员之间构成一棵树，如果某个职员参加舞会，那么他的直接下属不能参加。
 *   每个职员参加舞会会增加一定的快乐指数，求能够获得的最大快乐指数。
 *
 * 解法：树形动态规划
 *   对于每个职员，需要分别考虑他参加舞会和不参加舞会两种情况。
 *
 *   设 solve(node) 返回一个二元组：
 *     - first：node 参加舞会时，以 node 为根的子树能获得的最大快乐指数；
 *     - second：node 不参加舞会时，以 node 为根的子树能获得的最大快乐指数。
 *
 *   如果 node 参加舞会，那么所有直接下属都不能参加，
 *   每个子树只能取“子节点不参加”的状态：
 *     presentSum = val[node] + sum(absent[child])。
 *
 *   如果 node 不参加舞会，那么每个直接下属可以参加，也可以不参加，
 *   应选择其中快乐指数较大的状态：
 *     absentSum = sum(max(present[child], absent[child]))。
 *
 *   使用记忆化搜索保存已经计算过的子树结果，避免重复计算。
 *
 *   输入关系中 l 是下属，k 是 l 的直接上司，因此将 l 加入 k 的子节点列表。
 *   通过统计每个职员是否有上司，可以找到唯一的树根。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n)，每个职员和每条边只处理一次；
 *   - 空间复杂度：O(n)，用于存储树、入度和记忆化结果。
 */

#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

int n;

// val[i] 表示第 i 号职员参加舞会时增加的快乐指数
vector<int> val;

// te[i] 保存第 i 号职员的所有直接下属
vector<vector<int>> te;

// mp[node] 保存已经计算过的子树 DP 结果，避免重复递归
unordered_map<int, pair<int, int>> mp;

// 计算以 node 为根的子树在两种状态下的最大快乐指数
pair<int, int> solve(int node) {
    // 叶子节点没有下属：参加时获得自身快乐值，不参加时收益为 0
    if (te[node].empty()) {
        return {val[node], 0};
    }

    // 如果当前子树已经计算过，直接返回记忆化结果
    if (mp.find(node) != mp.end()) {
        return mp[node];
    }

    // absentSum：node 不参加舞会时的最大快乐指数
    // presentSum：node 参加舞会时的最大快乐指数
    int absentSum = 0, presentSum = val[node];

    // 合并所有直接下属子树的最优结果
    for (int v: te[node]) {
        auto [present, absent] = solve(v);

        // node 不参加时，子节点可以参加，也可以不参加，取较大值
        absentSum += max(present, absent);

        // node 参加时，子节点不能参加，只能选择 absent 状态
        presentSum += absent;
    }

    // 保存当前节点的两种状态并返回
    return mp[node] = {presentSum, absentSum};
}

int main() {
    // 读入职员总数
    cin >> n;

    // 初始化职员快乐指数和树结构
    val.assign(n + 1, 0);
    te.assign(n + 1, {});

    // 读入每个职员参加舞会时增加的快乐指数
    for (int i = 1; i <= n; i++) {
        cin >> val[i];
    }

    // in[i] 表示第 i 号职员是否有直接上司
    vector in(n + 1, 0);

    // 读入上下属关系：u 是下属，v 是 u 的直接上司
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        // 从上司 v 指向下属 u，便于从根节点向下进行 DFS
        te[v].push_back(u);
        in[u]++;
    }

    // 没有直接上司的职员就是整棵树的根节点
    int root = 0;
    for (int i = 1; i <= n; i++) {
        if (in[i] == 0) {
            root = i;
            break;
        }
    }

    // 计算根节点两种状态的最优值，并取其中较大者
    auto [present, absent] = solve(root);
    cout << max(present, absent) << endl;
    return 0;
}
