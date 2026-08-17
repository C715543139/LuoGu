/**
 * Luogu P1395 - 会议
 *
 * 问题描述：
 *   n 个村民的家通过 n-1 条长度为 1 的道路连成一棵树。选择一个节点作为会议地点，
 *   求所有节点到该节点的距离总和最小时的会议地点编号和最小距离和；若有多个地点
 *   满足条件，选择编号最小的节点。
 *
 * 解法：树形 DP + 换根 DP
 *   先以 1 号节点为根进行 dfs1。subtreeSize[u] 记录以 u 为根的子树节点数；递归
 *   参数 depth 是 u 到 1 号节点的距离，将所有 depth 累加后即可得到 sum[1]，即
 *   所有节点到 1 号节点的距离总和。
 *
 *   dfs2 将会议地点从父节点 parent 移到儿子 node。node 子树中的 subtreeSize[node]
 *   个节点到新会议地点的距离都减少 1，距离和减少 subtreeSize[node]；子树外的
 *   n-subtreeSize[node] 个节点到新会议地点的距离都增加 1，因此：
 *       sum[node] = sum[parent] + n - 2 * subtreeSize[node]
 *   沿树应用该公式后，可以得到以每个节点为会议地点时的距离总和。
 *
 *   最后按节点编号从小到大扫描 sum，只在发现严格更小的距离和时更新答案。距离和
 *   相同时会保留先扫描到的较小编号。
 *
 * 复杂度分析：
 *   两次 DFS 和一次答案扫描都只处理每个节点、每条边常数次：
 *   - 时间复杂度：O(n)
 *   - 空间复杂度：O(n)，其中递归栈最深可达 O(n)
 */

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

// n 是节点数，graph 使用邻接表保存无向树。
int n;
vector<vector<int>> graph;
// sum[u] 是所有节点到 u 的距离和；subtreeSize[u] 是 u 的子树大小。
vector<int> sum, subtreeSize;
// 两次 DFS 分别使用 visited，避免沿无向边返回已经访问的节点。
vector<bool> visited;

// 以 1 为根统计每个节点的子树大小，同时计算 sum[1]。
void dfs1(int node, int depth) {
    visited[node] = true;
    if (node != 1 && graph[node].size() == 1) {
        // 非根叶节点的子树只有它自己；depth 就是它对 sum[1] 的贡献。
        subtreeSize[node] = 1;
        sum[1] += depth;
        return;
    }
    for (int child: graph[node]) {
        if (!visited[child]) {
            // 儿子到根的距离比当前节点多 1；递归结束后累加儿子的子树大小。
            dfs1(child, depth + 1);
            subtreeSize[node] += subtreeSize[child];
        }
    }
    // 加上 node 自己，并将 node 到根的距离计入 sum[1]。
    subtreeSize[node]++;
    sum[1] += depth;
}

// 从 1 号节点向下换根，计算以每个节点为会议地点时的距离总和。
void dfs2(int node, int parent) {
    visited[node] = true;
    if (node != 1) {
        // 移到 node 后，其子树内每个节点的距离减 1，子树外每个节点的距离加 1。
        sum[node] = sum[parent] + n - 2 * subtreeSize[node];
    }
    for (int child: graph[node]) {
        if (!visited[child]) {
            // 当前 node 将作为 child 的父节点，用于下一次换根。
            dfs2(child, node);
        }
    }
}

int main() {
    // n 可达 5*10^4，关闭同步并解除绑定以加快输入输出。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    // 节点编号为 [1,n]，使用邻接表保存 n-1 条无向边。
    graph = vector<vector<int>>(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // 两个 DP 数组初始为 0；第一次 DFS 计算子树大小和 sum[1]。
    sum = subtreeSize = vector<int>(n + 1);
    visited = vector(n + 1, false);
    dfs1(1, 0);
    // 清空访问状态后进行第二次 DFS，利用换根公式计算其余 sum。
    visited = vector(n + 1, false);
    dfs2(1, 0);

    // idx 保存当前距离和最小的节点编号，初始选择 1 号节点。
    int idx = 1;
    for (int i = 1; i <= n; ++i) {
        if (sum[idx] > sum[i]) {
            // 只在严格更小时更新，因此距离和相同会保留编号较小的节点。
            idx = i;
        }
    }
    // sum[idx] 就是所有村民到最优会议地点的最小距离总和。
    cout << idx << ' ' << sum[idx] << endl;
    return 0;
}
