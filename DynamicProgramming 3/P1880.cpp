/**
 * Luogu P1880 - [NOI1995] 石子合并
 *
 * 问题描述：
 *   有若干堆石子围成一个环，每次只能合并相邻的两堆石子，
 *   合并得分为新合并石子的总数，求将所有石子合并成一堆的最小和最大得分。
 *
 * 解法：环形区间动态规划
 *   将长度为 n 的环形数组复制一份，得到长度为 2n 的数组。
 *   环上的任意连续 n 堆石子，都可以看作复制数组中的一个连续区间。
 *   因此，只需要求出所有长度为 n 的连续区间的合并结果，再取其中的最小值和最大值。
 *
 *   设 dpMin[i][j] 表示将区间 [i, j] 内的石子合并成一堆的最小得分，
 *   dpMax[i][j] 表示将区间 [i, j] 内的石子合并成一堆的最大得分。
 *
 *   考虑区间 [i, j] 的最后一次合并。设最后一次合并的分界点为 k，
 *   那么最后合并的两堆分别由区间 [i, k] 和 [k+1, j] 合并而来。
 *   最后一次合并的得分为整个区间内石子的总数，因此状态转移为：
 *     dpMin[i][j] = min(dpMin[i][k] + dpMin[k+1][j] + sum(i, j))；
 *     dpMax[i][j] = max(dpMax[i][k] + dpMax[k+1][j] + sum(i, j))。
 *
 *   使用前缀和数组 prefix，可以在 O(1) 时间内计算区间 [i, j] 的石子总数：
 *     sum(i, j) = prefix[j+1] - prefix[i]。
 *
 *   只有一堆石子时不需要进行合并，因此 dpMin[i][i] = dpMax[i][i] = 0。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n^3)，枚举区间长度、左端点和分界点；
 *   - 空间复杂度：O(n^2)。
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

int main() {
    // 读入石子堆数
    int n;
    cin >> n;

    // stones[i] 表示复制后第 i 堆石子的数量，下标从 0 开始
    vector<long long> stones(2 * n);

    // 将环形数组复制一份，使环上的连续区间变成数组中的普通连续区间
    for (int i = 0; i < n; ++i) {
        cin >> stones[i];
        stones[i + n] = stones[i];
    }

    // prefix[i] 表示 stones 前 i 个元素的石子总数
    vector<long long> prefix(2 * n + 1, 0);
    for (int i = 0; i < 2 * n; ++i) {
        prefix[i + 1] = prefix[i] + stones[i];
    }

    // 使用 long long 存储得分，并分别初始化最小值和最大值
    const long long inf = numeric_limits<long long>::max();
    vector dpMin(2 * n, vector(2 * n, inf));
    vector dpMax(2 * n, vector(2 * n, -inf));

    // 合并一堆石子不需要进行任何操作，得分为 0
    for (int i = 0; i < 2 * n; ++i) {
        dpMin[i][i] = 0;
        dpMax[i][i] = 0;
    }

    // 按区间长度从小到大计算，保证转移所需的子区间已经求出
    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i + len <= 2 * n; ++i) {
            int j = i + len - 1;

            // 计算当前区间 [i, j] 内的石子总数
            long long sum = prefix[j + 1] - prefix[i];

            // 枚举最后一次合并的分界点 k
            for (int k = i; k < j; ++k) {
                // 左右子区间分别合并完成后，再合并整个区间
                dpMin[i][j] = min(dpMin[i][j],
                                  dpMin[i][k] + dpMin[k + 1][j] + sum);
                dpMax[i][j] = max(dpMax[i][j],
                                  dpMax[i][k] + dpMax[k + 1][j] + sum);
            }
        }
    }

    // 枚举环形数组的起点，选择所有长度为 n 的区间中的最小值和最大值
    long long answerMin = inf;
    long long answerMax = -inf;
    for (int i = 0; i < n; ++i) {
        answerMin = min(answerMin, dpMin[i][i + n - 1]);
        answerMax = max(answerMax, dpMax[i][i + n - 1]);
    }

    // 输出最小得分和最大得分
    cout << answerMin << '\n' << answerMax << '\n';
    return 0;
}
