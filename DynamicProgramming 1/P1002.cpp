/**
 * Luogu P1002 - [NOIP 2002 普及组] 过河卒
 *
 * 问题描述：
 *   棋盘上 A(0,0) 处有一个过河卒，只能向下或向右走，目标是 B(n,m)。棋盘上还有一匹
 *   固定的马，马所在位置及其一步可跳到的 8 个位置是控制点，卒不能经过这些点。求卒
 *   从 A 到 B 且不经过控制点的路径条数。
 *
 * 解法：递推动态规划
 *   设 dp[i][j] 表示从起点走到 (i,j) 的路径条数。卒只能从上方或左方走入当前位置，
 *   因此普通位置的转移为 dp[i][j] = dp[i-1][j] + dp[i][j-1]。若 (i,j) 是马的控制点，
 *   则该点路径数为 0（不可到达）。为避免处理边界，网格整体向右、向下平移一格，把
 *   dp[1][1] 对应起点 (0,0)，且 dp[0][*]、dp[*][0] 全为 0 充当哨兵。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(nm)
 *   - 空间复杂度：O(nm)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int xa, ya, xb, yb;
    // xa, ya 为目标点 B 的坐标，xb, yb 为马的坐标。
    cin >> xa >> ya >> xb >> yb;

    // dp[i][j] 保存走到 (i-1, j-1) 的路径条数，整体平移一格留出哨兵边界，用 long long 防止溢出。
    vector dp(xa + 2, vector(ya + 2, 0ll));

    // check(x,y) 判断 (x,y) 是否为马的控制点：马所在位置或马一步可跳到的 8 个位置。
    auto check = [&](int x, int y) {
        if (x == xb + 2 && y == yb + 1
            || x == xb + 1 && y == yb + 2
            || x == xb - 1 && y == yb + 2
            || x == xb - 2 && y == yb + 1
            || x == xb - 2 && y == yb - 1
            || x == xb - 1 && y == yb - 2
            || x == xb + 1 && y == yb - 2
            || x == xb + 2 && y == yb - 1
            || x == xb && y == yb) {
            return true;
        }
        return false;
    };

    // 起点 (0,0) 对应 dp[1][1]，到达它的路径有 1 条。
    dp[1][1] = 1;
    for (int i = 0; i <= xa; i++) {
        for (int j = 0; j <= ya; j++) {
            // 起点已经初始化过，跳过。
            if (i == 0 && j == 0) {
                continue;
            }
            // 非控制点才能从上方或左方走入；控制点保持 0（不可到达）。
            if (!check(i, j)) {
                dp[i + 1][j + 1] = dp[i][j + 1] + dp[i + 1][j];
            }
        }
    }
    // dp[xa+1][ya+1] 对应目标点 B(xa, ya)。
    cout << dp[xa + 1][ya + 1] << endl;
    return 0;
}
