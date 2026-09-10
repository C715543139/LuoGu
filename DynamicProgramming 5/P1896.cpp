/**
 * Luogu P1896 - [SCOI2005] 互不侵犯
 *
 * 问题描述：
 *   在 n x n 的棋盘中放置恰好 k 个国王。任意两个国王不能位于相邻格子，
 *   即横向、纵向和斜向均不能相邻，求合法摆放方案数。
 *
 * 解法：状态压缩 + 记忆化搜索
 *   使用一个 n 位二进制整数表示一行的摆放状态，第 c 位为 1 表示该行第 c 列
 *   放置了国王。首先筛选出所有满足 state & (state << 1) == 0 的状态，
 *   保证同一行内不存在左右相邻的国王。
 *
 *   设 solve(i, j, s) 表示：
 *   - 当前准备处理第 i 行；
 *   - 第 i-1 行的状态为 j；
 *   - 前 i 行已经放置了 s 个国王；
 *   在此基础上放满剩余各行并最终恰好放置 k 个国王的方案数。
 *
 *   枚举第 i 行的合法状态 currentState，它与上一行状态 j 必须满足：
 *   - currentState & j == 0：上下不能相邻；
 *   - currentState & (j << 1) == 0：一个方向的斜对角不能相邻；
 *   - currentState & (j >> 1) == 0：另一个方向的斜对角不能相邻。
 *   满足限制后递归处理下一行，并将各个选择的方案数相加。
 *
 * 复杂度分析：
 *   设合法行状态数量为 S，S <= 2^n。
 *   - 时间复杂度：O(n * k * S^2)；
 *   - 空间复杂度：O(n * k * 2^n)。
 */

#include <iostream>
#include <vector>

using namespace std;

int n, k;
// 所有行状态的总数为 2^n
int stateCount;
// kingCount[state] 表示状态 state 中国王的数量
vector<int> kingCount;
// validStates 保存同一行内没有相邻国王的状态
vector<int> validStates;
// memo[i][j][s] 缓存 solve(i, j, s) 的计算结果
vector<vector<vector<long long>>> memo;

// prevState 和 currentState 分别表示相邻两行的摆放状态。
bool canFollow(int prevState, int currentState) {
    // 同列不能相邻，左右斜对角也不能相邻。
    return (currentState & prevState) == 0 &&
           (currentState & (prevState << 1)) == 0 &&
           (currentState & (prevState >> 1)) == 0;
}

// 处理第 i 行，上一行状态为 j，已经放置 s 个国王。
long long solve(int i, int j, int s) {
    // 已放置的国王超过 k 个，当前方案不合法
    if (s > k) {
        return 0;
    }

    // 所有行均处理完成，恰好放置 k 个国王时得到一种合法方案
    if (i == n) {
        return s == k;
    }

    // 当前状态已经计算过，直接返回缓存结果
    long long &answer = memo[i][j][s];
    if (answer != -1) {
        return answer;
    }

    answer = 0;
    // 枚举当前行所有内部合法的摆放状态
    for (int currentState: validStates) {
        int added = kingCount[currentState];

        // 国王总数不能超过 k，当前行还必须与上一行互不侵犯
        if (s + added > k || !canFollow(j, currentState)) {
            continue;
        }

        // 处理下一行，并累加当前摆法对应的方案数
        answer += solve(i + 1, currentState, s + added);
    }
    return answer;
}

int main() {
    // 读入棋盘边长和需要放置的国王数量
    cin >> n >> k;

    stateCount = 1 << n;
    kingCount.resize(stateCount);

    // 只保留同一行没有相邻国王的状态。
    for (int state = 0; state < stateCount; ++state) {
        kingCount[state] = __builtin_popcount(state);
        if ((state & (state << 1)) == 0) {
            validStates.push_back(state);
        }
    }

    // -1 表示对应的搜索状态尚未计算
    memo.assign(n, vector(stateCount, vector<long long>(k + 1, -1)));

    // 第 0 行的上一行视为空行，状态为 0。
    cout << solve(0, 0, 0) << '\n';
    return 0;
}
