/**
 * Luogu P2216 - [HAOI2007] 理想的正方形
 *
 * 问题描述：
 *   在 a×b 的整数矩阵中，寻找一个 n×n 正方形，
 *   使正方形内最大值与最小值之差最小。
 *
 * 解法：两次单调队列
 *   1. 横向扫描每一行，分别求出所有宽度为 n 的区间最小值 rowMin 与最大值 rowMax。
 *   2. 固定横向区间起点，对 rowMin 和 rowMax 再做一次高度为 n 的纵向滑动窗口：
 *      - n×n 正方形的最小值 = 这 n 行 rowMin 的最小值；
 *      - n×n 正方形的最大值 = 这 n 行 rowMax 的最大值。
 *   3. 枚举所有正方形，更新最小的最大值减最小值。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(a×b)，每个元素在横向和纵向队列中最多入队、出队各一次
 *   - 空间复杂度：O(a×b)，保存原矩阵及每行滑动窗口的最小值、最大值
 */

#include <algorithm>
#include <climits>
#include <deque>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b, n;
    cin >> a >> b >> n;

    // 读入原矩阵
    vector matrix(a, vector(b, 0));
    for (int i = 0; i < a; ++i) {
        for (int j = 0; j < b; ++j) {
            cin >> matrix[i][j];
        }
    }

    // 宽度为 n 的横向窗口共有 b-n+1 个起点
    int windowCount = b - n + 1;
    // rowMin[i][j]、rowMax[i][j]：第 i 行区间 [j, j+n-1] 的最小值和最大值
    vector rowMin(a, vector(windowCount, 0));
    vector rowMax(a, vector(windowCount, 0));

    // 第一次单调队列：逐行计算宽度为 n 的最小值和最大值
    for (int i = 0; i < a; ++i) {
        deque<int> minQueue, maxQueue;  // 队列中保存列下标
        for (int j = 0; j < b; ++j) {
            // 维护递增队列，队首对应当前横向窗口最小值
            while (!minQueue.empty() && matrix[i][minQueue.back()] >= matrix[i][j]) {
                minQueue.pop_back();
            }
            minQueue.push_back(j);

            // 维护递减队列，队首对应当前横向窗口最大值
            while (!maxQueue.empty() && matrix[i][maxQueue.back()] <= matrix[i][j]) {
                maxQueue.pop_back();
            }
            maxQueue.push_back(j);

            // 移除左侧已经离开长度为 n 窗口的列
            while (minQueue.front() <= j - n) minQueue.pop_front();
            while (maxQueue.front() <= j - n) maxQueue.pop_front();

            // 当前窗口为 [j-n+1, j]，记录它的横向极值
            if (j >= n - 1) {
                rowMin[i][j - n + 1] = matrix[i][minQueue.front()];
                rowMax[i][j - n + 1] = matrix[i][maxQueue.front()];
            }
        }
    }

    long long answer = LLONG_MAX;
    // 第二次单调队列：固定横向窗口起点，纵向求高度为 n 的极值
    for (int j = 0; j < windowCount; ++j) {
        deque<int> minQueue, maxQueue;  // 队列中保存行下标
        for (int i = 0; i < a; ++i) {
            // rowMin 的递增队列，队首为 n 行中的最小值所在行
            while (!minQueue.empty() && rowMin[minQueue.back()][j] >= rowMin[i][j]) {
                minQueue.pop_back();
            }
            minQueue.push_back(i);

            // rowMax 的递减队列，队首为 n 行中的最大值所在行
            while (!maxQueue.empty() && rowMax[maxQueue.back()][j] <= rowMax[i][j]) {
                maxQueue.pop_back();
            }
            maxQueue.push_back(i);

            // 移除上侧已经离开高度为 n 窗口的行
            while (minQueue.front() <= i - n) minQueue.pop_front();
            while (maxQueue.front() <= i - n) maxQueue.pop_front();

            // 当前正方形覆盖行 [i-n+1, i]、列 [j, j+n-1]
            if (i >= n - 1) {
                long long minimum = rowMin[minQueue.front()][j];
                long long maximum = rowMax[maxQueue.front()][j];
                answer = min(answer, maximum - minimum);
            }
        }
    }

    cout << answer << endl;
    return 0;
}
