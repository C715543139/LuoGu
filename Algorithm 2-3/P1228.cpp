/**
 * Luogu P1228 - 地毯填补问题
 *
 * 问题描述：
 *   在一个边长为 2^k 的正方形棋盘中保留一个指定格子，其余格子用 L 形地毯恰好覆盖一次。
 *
 * 解法：四分递归
 *   将当前边长为 2^k 的棋盘分为四个边长为 2^(k - 1) 的象限。
 *   特殊格子位于其中一个象限，先在棋盘中央放置一块 L 形地毯，覆盖其余三个象限的中心角格。
 *   这三个已覆盖的中心角格在各自的子问题中视为“不能再覆盖的特殊格”，
 *   从而四个象限都能递归地转化为同一个填补问题。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(4^k)，与输出地毯数量同阶
 *   - 递归栈空间复杂度：O(k)
 */

#include <iostream>
using namespace std;

// 填补左上角为 (startX, startY)、边长为 2^k 的子棋盘。
// (x, y) 是该子棋盘中不能再覆盖的格子：最初为公主所在格，递归时也可能是父地毯已覆盖的格子。
void solve(int x, int y, int startX, int startY, int k) {
    // k = 1 时棋盘大小为 2 x 2，一块 L 形地毯即可覆盖除特殊格外的三个格子。
    if (k == 1) {
        int targetX, targetY, i;
        if (x == startX && y == startY) {
            // 特殊格在左上角：按题目形状编号输出对应的地毯拐角和形状 1。
            targetX = startX + 1, targetY = startY + 1, i = 1;
        } else if (x == startX && y == startY + 1) {
            // 特殊格在右上角。
            targetX = startX + 1, targetY = startY, i = 2;
        } else if (x == startX + 1 && y == startY) {
            // 特殊格在左下角。
            targetX = startX, targetY = startY + 1, i = 3;
        } else {
            // 特殊格在右下角。
            targetX = startX, targetY = startY, i = 4;
        }
        cout << targetX << ' ' << targetY << ' ' << i << '\n';
        return;
    }

    // l 是每个象限的边长，pos 记录特殊格所属象限：0 左上，1 右上，2 左下，3 右下。
    int pos;
    int l = 1;
    for (int i = 0; i < k - 1; ++i) l *= 2;
    if (x < startX + l && y < startY + l) {
        pos = 0;
    } else if (x >= startX + l && y < startY + l) {
        pos = 2;
    } else if (x < startX + l && y >= startY + l) {
        pos = 1;
    } else {
        pos = 3;
    }

    // 先确定中央 L 形地毯的输出参数。
    // 它恰好不覆盖 pos 象限的中心角，并覆盖另外三个象限的中心角。
    int targetX, targetY, i;
    if (pos == 0) {
        targetX = startX + l, targetY = startY + l, i = 1;

        // 左上象限保留原特殊格；其他三个象限以其被中央地毯覆盖的中心角作为特殊格递归。
        solve(x, y, startX, startY, k - 1);
        solve(startX + l - 1, startY + l, startX, startY + l, k - 1);
        solve(startX + l, startY + l - 1, startX + l, startY, k - 1);
        solve(startX + l, startY + l, startX + l, startY + l, k - 1);
    } else if (pos == 1) {
        targetX = startX + l, targetY = startY + l - 1, i = 2;

        // 右上象限保留原特殊格。
        solve(x, y, startX, startY + l, k - 1);
        solve(startX + l - 1, startY + l - 1, startX, startY, k - 1);
        solve(startX + l, startY + l - 1, startX + l, startY, k - 1);
        solve(startX + l, startY + l, startX + l, startY + l, k - 1);
    } else if (pos == 2) {
        targetX = startX + l - 1, targetY = startY + l, i = 3;

        // 左下象限保留原特殊格。
        solve(x, y, startX + l, startY, k - 1);
        solve(startX + l - 1, startY + l - 1, startX, startY, k - 1);
        solve(startX + l - 1, startY + l, startX, startY + l, k - 1);
        solve(startX + l, startY + l, startX + l, startY + l, k - 1);
    } else {
        targetX = startX + l - 1, targetY = startY + l - 1, i = 4;

        // 右下象限保留原特殊格。
        solve(x, y, startX + l, startY + l, k - 1);
        solve(startX + l - 1, startY + l - 1, startX, startY, k - 1);
        solve(startX + l - 1, startY + l, startX, startY + l, k - 1);
        solve(startX + l, startY + l - 1, startX + l, startY, k - 1);
    }

    // 父问题的中央地毯可在子问题前后任意输出；这里沿用原实现，在四个子问题之后输出。
    cout << targetX << ' ' << targetY << ' ' << i << '\n';
}

int main() {
    int k, x, y;
    cin >> k >> x >> y;

    // 整个棋盘左上角为 (1, 1)，边长为 2^k。
    solve(x, y, 1, 1, k);
    return 0;
}
