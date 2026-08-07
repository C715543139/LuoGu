/**
 * Luogu P2032 - 扫描
 *
 * 问题描述：
 *   对长度为 n 的序列，依次输出每个长度为 k 的连续窗口中的最大值。
 *
 * 解法：大根堆 + 窗口下标判断
 *   1. 大根堆 pq 保存 (数值, 下标) 二元组，按数值从大到小排列。
 *   2. 处理到下标 i 时，当前窗口为 [i-k+1, i]。
 *      若堆顶元素的下标小于窗口左端点，说明它已过期，直接弹出。
 *   3. 清理完所有过期堆顶后，堆顶元素仍在窗口内，且其数值就是窗口最大值。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n log n)，每个元素最多入堆、出堆一次
 *   - 空间复杂度：O(n)，堆中可能暂存尚未弹出的过期元素
 */

#include <iostream>
#include <queue>
#include <utility>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    // 大根堆元素为 {数值, 下标}，可通过下标直接判断是否仍在窗口内
    priority_queue<pair<int, int>> pq;
    // 逐个读入元素；处理到 i 时，当前窗口右端点为 i
    for (int i = 0; i < n; i++) {
        int value;
        cin >> value;
        pq.emplace(value, i);

        if (i < k - 1) continue;  // 尚未凑满第一个长度为 k 的窗口

        // 当前窗口为 [i-k+1, i]，弹出窗口左侧已经过期的堆顶元素
        while (pq.top().second < i - k + 1) {
            pq.pop();
        }
        // 清理后堆顶数值即为当前窗口最大值
        cout << pq.top().first << '\n';
    }
    return 0;
}
