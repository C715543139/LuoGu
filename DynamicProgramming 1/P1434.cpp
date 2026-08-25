/**
 * Luogu P1434 - [SHOI2002] 滑雪
 *
 * 问题描述：
 *   区域由一个二维数组表示，每个数字代表该点的高度。可以从某个点滑向上下左右相邻
 *   四个点之一，当且仅当高度会减小。求整个区域中最长滑坡的长度。
 *
 * 解法：记忆化搜索（自顶向下的动态规划）
 *   设 memory[i][j] 表示从位置 (i,j) 出发、沿高度严格递减的方向能滑出的最长长度。
 *   从 (i,j) 可以滑向任意一个高度更低的相邻位置，因此：
 *       memory[i][j] = max(1, max{ memory[x][y] + 1 })，其中 (x,y) 是 (i,j) 的高度
 *   更低的相邻位置。高度只会减小，不会出现环，因此用递归加上记忆化即可避免重复计算。
 *   数组最外层用高度 10001 作哨兵，保证边界位置不会向数组外移动。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(RC)，每个位置最多被计算一次
 *   - 空间复杂度：O(RC)
 */

#include <iostream>
#include <vector>
using namespace std;

// v[i][j] 保存位置 (i,j) 的高度；memory[i][j] 保存该位置出发的最长滑坡长度，-1 表示尚未计算。
vector<vector<int>> v;
vector<vector<int>> memory;
// maxRes 记录遍历过程中出现的最长滑坡长度，初始至少为 1。
int maxRes = 1;

// 返回从 (i,j) 出发能滑出的最长长度，同时用记忆化避免重复计算。
int solve(int i, int j) {
    if (memory[i][j] != -1) {
        // 已经计算过，直接返回缓存的结果。
        return memory[i][j];
    }

    // h 为当前高度，lh/rh/uh/dh 分别为左、右、上、下四个相邻位置的高度。
    int h = v[i][j], lh = v[i][j - 1], rh = v[i][j + 1], uh = v[i - 1][j], dh = v[i + 1][j];
    if (h <= lh && h <= rh && h <= uh && h <= dh) {
        // 四周都不更低，无法继续下滑，只能停在这里。
        memory[i][j] = 1;
        return 1;
    }

    // res 引用 memory[i][j]，向下递归时把最长结果逐步写回缓存。
    int &res = memory[i][j];
    if (h > lh) {
        res = solve(i, j - 1) + 1;
    }
    if (h > rh) {
        res = max(res, solve(i, j + 1) + 1);
    }
    if (h > uh) {
        res = max(res, solve(i - 1, j) + 1);
    }
    if (h > dh) {
        res = max(res, solve(i + 1, j) + 1);
    }
    // 更新全局最优答案。
    maxRes = max(maxRes, res);
    return res;
}

int main() {
    int r, c;
    cin >> r >> c;
    // 数组四周各多留一行一列作为哨兵，初始高度设为 10001（大于任何真实高度）。
    v.assign(r + 2, vector(c + 2, 10001));
    memory.assign(r + 2, vector(c + 2, -1));
    for (int i = 1; i <= r; ++i) {
        for (int j = 1; j <= c; ++j) {
            cin >> v[i][j];
        }
    }

    // 依次以每个位置为起点计算，由 solve 内部维护全局最优答案。
    for (int i = 1; i <= r; ++i) {
        for (int j = 1; j <= c; ++j) {
            solve(i, j);
        }
    }
    cout << maxRes << endl;
    return 0;
}
