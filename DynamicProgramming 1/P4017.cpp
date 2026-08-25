/**
 * Luogu P4017 - 最大食物链计数
 *
 * 问题描述：
 *   给定一个食物网，求其中"最大食物链"的数量。"最大食物链"指生物学意义上的完整
 *   食物链：左端是生产者（不捕食其他生物），右端是消费者（不被其他生物捕食），
 *   中间按吃与被吃的关系逐级相连。结果对 80112002 取模。
 *
 * 解法：记忆化搜索（自顶向下的动态规划）
 *   读入时把关系"被吃的生物 u 被生物 v 捕食"建成有向边 v -> u，即从捕食者指向
 *   被捕食者，于是 graph[u] 保存 u 捕食的全部猎物。设 memory[u] 表示以 u 为末端、
 *   从某个生产者沿捕食关系延伸过来的链的条数：若 u 不捕食任何生物，它就是生产者，
 *   这样的链只有它自己一条，记作 1；否则链的倒数第二个位置必然是 u 捕食的某个
 *   猎物 v，于是：
 *       memory[u] = sum(memory[v])，其中 v 是被 u 捕食的生物
 *   由于数据保证无环，沿"捕食"方向不会回头，递归加记忆化即可。inDegree[u] 表示
 *   捕食 u 的生物数量（即 u 被吃的次数），inDegree[u] == 0 说明 u 是不被任何生物
 *   捕食的消费者，也就是完整食物链的最右端，把所有这样的 memory[u] 相加即为答案。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n + m)，每个节点和每条边各处理一次
 *   - 空间复杂度：O(n + m)
 */

#include <iostream>
#include <vector>
using namespace std;

// graph[u] 保存 u 捕食的全部猎物；memory[u] 保存以 u 为末端的链的条数，0 表示尚未
// 计算；inDegree[u] 保存捕食 u 的生物数量（即 u 被吃的次数）。
vector<vector<int>> graph;
vector<int> memory, inDegree;

// 返回以 u 为末端、从某个生产者沿捕食关系延伸过来的链的条数，并用 memory 记忆化。
int solve(int u) {
    if (graph[u].empty()) {
        // u 不捕食任何生物，是生产者，这样的链只有它自己一条。
        memory[u] = 1;
        return 1;
    }

    // res 引用 memory[u]，若已经计算过则直接返回缓存结果。
    int &res = memory[u];
    if (res != 0) {
        return res;
    }

    for (int v: graph[u]) {
        // v 是被 u 捕食的生物，捕食 v 的生物数量加一。
        inDegree[v]++;
        // 链的倒数第二个位置是某个猎物 v，累加所有分支的条数并对模数取余。
        res = (res % 80112002 + solve(v) % 80112002) % 80112002;
    }
    return res;
}

int main() {
    int n, m;
    cin >> n >> m;
    inDegree.assign(n + 1, 0);
    memory.assign(n + 1, 0);
    graph.assign(n + 1, {});
    for (int i = 0; i < m; i++) {
        // 读入"被吃的生物 u 被生物 v 捕食"，建成从捕食者 v 指向猎物 u 的有向边。
        int u, v;
        cin >> u >> v;
        graph[v].push_back(u);
    }

    // 对每个生物求一次记忆化搜索，solve 内部会顺便统计每个生物的 inDegree。
    for (int u = 1; u <= n; u++) {
        solve(u);
    }

    int res = 0;
    for (int u = 1; u <= n; u++) {
        // 不被任何生物捕食的消费者才是完整食物链的最右端，累加以其为末端的链数。
        if (inDegree[u] == 0) {
            res = (res % 80112002 + memory[u] % 80112002) % 80112002;
        }
    }
    cout << res << endl;
    return 0;
}
