/**
 * Luogu B3611 - 【模板】传递闭包
 *
 * 问题描述：
 *   给定一张有向图的邻接矩阵，求它的传递闭包。输出矩阵的第 i 行第 j 列
 *   表示顶点 i 能否经过一条或多条有向边到达顶点 j。
 *
 * 解法：枚举起点 + BFS
 *   先把输入的邻接矩阵转换成邻接表。随后枚举每个顶点 i 作为起点，
 *   从 i 执行一次 BFS。搜索中被访问到的顶点 j 都满足 i 可以到达 j，
 *   因此该次搜索的 visited 数组就是传递闭包的第 i 行。
 *
 *   BFS 开始时只将起点 i 入队，没有立即设置 visited[i]=true。这样只有存在
 *   一条从 i 出发又回到 i 的环时，结果矩阵的对角元素 mt[i][i] 才会为 1。
 *
 * 复杂度分析：
 *   设图中实际有向边数为 E。
 *   - 读取邻接矩阵：O(n^2)
 *   - n 次 BFS：O(n(n+E))，稠密图中最坏为 O(n^3)
 *   - 空间复杂度：O(n^2+E)，可记为 O(n^2)
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    // graph[i] 保存顶点 i 的所有出边终点，顶点在程序中使用 0 开始的编号。
    vector<vector<int>> graph(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int x;
            cin >> x;
            // 邻接矩阵中的 1 表示存在有向边 i -> j。
            if (x) {
                graph[i].push_back(j);
            }
        }
    }

    // mt[i][j] 表示顶点 i 是否能直接或间接到达顶点 j。
    vector<vector<bool>> mt(n);
    // 分别从每个顶点出发做一次 BFS，求出传递闭包的一行。
    for (int i = 0; i < n; ++i) {
        // visited[v] 表示本次搜索已经从起点 i 到达过 v。
        vector visited(n, false);
        queue<int> q;
        // 此处不直接标记 visited[i]，以便只在真正存在回到 i 的环时令 mt[i][i]=1。
        q.push(i);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            // 沿 u 的所有出边继续扩展可达范围。
            for (int v: graph[u]) {
                // 每个已达顶点只需入队一次，避免在环中重复搜索。
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        // 保存从 i 出发的全部可达性结果。
        mt[i] = visited;
    }

    // 按邻接矩阵的形式输出传递闭包。
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << (mt[i][j] ? 1 : 0) << ' ';
        }
        cout << '\n';
    }
    return 0;
}
