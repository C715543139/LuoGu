/**
 * Luogu P5908 - 猫猫和企鹅
 *
 * 问题描述：
 *   n 个居住区通过 n-1 条道路连成一棵树，每条道路长度均为 1。1 号居住区没有
 *   企鹅，其余每个居住区各有一只企鹅。求从 1 号居住区出发，距离不超过 d 的
 *   居住区数量，也就是猫猫能够拜访的企鹅数量。
 *
 * 解法：分层广度优先搜索
 *   从 1 号节点开始逐层访问树。cur 保存当前距离层中的所有节点；扫描 cur 中节点
 *   的邻接点后，next 恰好保存下一距离层中尚未访问的节点。由于所有边长度均为 1，
 *   第 i 轮找到的 next 中所有节点到 1 号节点的距离都为 i+1。
 *
 *   使用 visited 保证每个节点只会进入一次距离层，同时避免沿无向边返回父节点。
 *   每完成一层搜索，将 next.size() 计入答案，再交换 cur 和 next。最多扩展 d 层，
 *   便可统计距离为 1 到 d 的所有节点；起点 1 不会被计入答案。
 *
 * 复杂度分析：
 *   每个节点至多访问一次，每条边至多从两个方向各检查一次：
 *   - 时间复杂度：O(n)
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    // n 可达 10^5，关闭同步并解除绑定以加快输入输出。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, d;
    cin >> n >> d;
    // 使用邻接表保存这棵无向树，节点编号范围为 [1,n]。
    vector<vector<int>> graph(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        // 一条道路可以从两个方向通行，因此需要加入两次。
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // res 统计距离 1 号节点不超过 d 的非起点节点数。
    long long res = 0;
    // 节点加入某一距离层时立即标记，防止它被其他相邻节点重复加入。
    vector visited(n + 1, false);
    // cur 保存当前距离层，next 保存搜索得到的下一距离层。
    vector<int> cur, next;
    // 搜索从距离为 0 的起点 1 开始；起点本身没有企鹅，不计入 res。
    cur.push_back(1);
    visited[1] = true;
    for (int i = 0; i < d; i++) {
        // 本轮开始时，cur 中所有节点到起点的距离都为 i。
        for (int u: cur) {
            for (int v: graph[u]) {
                if (!visited[v]) {
                    // 首次发现的邻接点距离为 i+1，将其放入下一层。
                    next.push_back(v);
                    visited[v] = true;
                }
            }
        }
        // 下一层为空说明已经没有更远的节点，可以提前结束搜索。
        if (next.empty()) {
            break;
        }
        // next 中每个节点都对应一只距离不超过 d 的企鹅。
        res += next.size();
        // 交换后 cur 成为下一轮要处理的层，next 暂时保存刚处理完的旧层。
        cur.swap(next);
        // 清除旧层节点，使 next 可以在下一轮中继续收集新的节点。
        next.clear();
    }
    // 1 号节点从未计入 res，因此结果恰好是能够拜访的企鹅数量。
    cout << res << endl;
    return 0;
}
