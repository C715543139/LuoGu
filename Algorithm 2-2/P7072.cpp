/**
 * Luogu P7072 - [CSP-J 2020] 直播获奖
 *
 * 问题描述：
 *   按顺序公布 n 位选手的成绩。公布前 p 位成绩后，计划获奖人数为
 *   max(1, floor(p * w / 100))。
 *   输出每个时刻使计划获奖人数进入前列的最低分数线；同分选手可同时获奖。
 *
 * 解法：计数桶
 *   1. 所有成绩均在 [0, 600] 内，bucket[s] 记录当前分数恰为 s 的选手人数。
 *   2. 每读入一名选手，先更新对应的桶，再用整数运算计算计划获奖人数。
 *   3. 从 600 向下累加人数。首次使累计人数达到计划获奖人数的分数，
 *      就是当前分数线；该做法会自然保留全部同分选手。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(601n)，分数范围为常数，可视为 O(n)
 *   - 空间复杂度：O(601)，可视为 O(1)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, w;
    cin >> n >> w;

    // bucket[s]：当前已公布成绩中，分数恰好为 s 的选手人数
    vector bucket(601, 0);
    // 依次处理每位选手，处理后立即输出当前分数线
    for (int i = 0; i < n; ++i) {
        int t;
        cin >> t;
        // 将新公布的成绩加入计数桶
        bucket[t]++;

        // 当前计划获奖人数：至少为 1，使用整数运算避免浮点误差
        int index = max(1, w * (i + 1) / 100);
        // count：分数大于等于当前 j 的选手总数
        for (int j = 600, count = 0; j >= 0; --j) {
            count += bucket[j];
            if (count >= index) {
                // 第一个满足累计人数要求的分数即为即时获奖分数线
                cout << j << " ";
                break;
            }
        }
    }
    return 0;
}
