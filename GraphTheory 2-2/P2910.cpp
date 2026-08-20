/**
 * Luogu P2910 - [USACO08OPEN] Clear And Present Danger S
 *
 * 问题描述：
 *   给定 n 座岛屿之间的有向航线危险指数，以及必须按顺序经过的 m 座岛屿
 *   A_1,A_2,...,A_m。求依次满足这些经过要求时，整条航行路线的最小危险指数之和。
 *
 * 解法：Floyd 全源最短路
 *   原始危险指数以邻接矩阵给出，而且不同方向的危险指数可能不同，
 *   因此 dist[i][j] 表示从岛屿 i 到岛屿 j 的当前最小危险指数。
 *
 *   Floyd 依次枚举岛屿 k 作为新允许使用的中转点。对任意 i,j，比较原有路线
 *   i -> j 和经过 k 的路线 i -> k -> j，并保留危险指数较小者。算法结束后，
 *   dist[i][j] 就是任意两座岛屿之间的最小危险指数。
 *
 *   完整路线可以按必经岛屿划分为 A_i -> A_(i+1) 的 m-1 段。每段都选择
 *   对应两座岛屿间的最短路，再将各段 dist[A_i][A_(i+1)] 相加，即得到全局最优解。
 *
 * 复杂度分析：
 *   - Floyd 时间复杂度：O(n^3)
 *   - 读取和累加必经序列：O(m)
 *   - 空间复杂度：O(n^2+m)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    // sq 按输入顺序保存必须经过的岛屿编号，这里仍保留题目的 1 开始编号。
    vector sq(m, 0);
    for (int i = 0; i < m; ++i) {
        cin >> sq[i];
    }
    // dist[i][j] 初始为从岛屿 i 直接到岛屿 j 的危险指数。
    // 程序内部的岛屿使用 0 开始的下标。
    vector dist(n, vector(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> dist[i][j];
        }
    }

    // 枚举新允许作为中转点的岛屿 k，k 必须位于最外层。
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // 比较直接从 i 到 j，与经过 k 中转的 i -> k -> j。
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    // 答案是序列中每对相邻必经岛屿之间最短距离的总和。
    // m 最大为 10000，使用 long long 保存累加结果。
    long long sum = 0;
    for (int i = 0; i < m - 1; ++i) {
        // 将题目的 1 开始岛屿编号转换为 dist 的 0 开始下标。
        int x = sq[i] - 1, y = sq[i + 1] - 1;
        sum += dist[x][y];
    }
    cout << sum << endl;
    return 0;
}
