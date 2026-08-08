/**
 * Luogu P1886 - 【模板】单调队列 / 滑动窗口
 *
 * 问题描述：
 *   给定长度为 n 的序列和长度为 k 的滑动窗口，
 *   输出每个窗口中的最小值与最大值。
 *
 * 解法：双单调队列
 *   1. minDQ 保存候选元素的下标，对应数值从队首到队尾单调不减，
 *      队首始终是当前窗口最小值的下标。
 *   2. maxDQ 保存候选元素的下标，对应数值从队首到队尾单调不增，
 *      队首始终是当前窗口最大值的下标。
 *   3. 新元素进入时，删除队尾不可能再成为极值的元素；
 *      窗口右移时，删除队首已经离开窗口的下标。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n)，每个下标在每个队列中最多入队、出队各一次
 *   - 空间复杂度：O(n)，保存输入与全部窗口结果；两个队列额外占 O(k)
 */

#include <deque>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    // 读入原序列
    vector v(n, 0);
    for (int i = 0; i < n; i++) cin >> v[i];

    // 分别保存各窗口的最小值和最大值，预留容量避免反复扩容
    vector<int> minRes, maxRes;
    minRes.reserve(n - k + 1);
    maxRes.reserve(n - k + 1);
    // 队列中保存下标，便于按窗口左端点判断元素是否过期
    deque<int> minDQ, maxDQ;
    // 初始化第一个窗口 [0, k-1]
    for (int i = 0; i < k; i++) {
        // 移除队尾中数值不小于当前值的候选，当前值更晚离开窗口
        while (!minDQ.empty() && v[i] <= v[minDQ.back()]) {
            minDQ.pop_back();
        }
        minDQ.push_back(i);
        // 移除队尾中数值不大于当前值的候选，当前值更晚离开窗口
        while (!maxDQ.empty() && v[i] >= v[maxDQ.back()]) {
            maxDQ.pop_back();
        }
        maxDQ.push_back(i);
    }
    // 两个队首分别对应第一个窗口的最小值和最大值
    minRes.push_back(v[minDQ.front()]);
    maxRes.push_back(v[maxDQ.front()]);

    // 每轮将窗口从 [i, i+k-1] 右移到 [i+1, i+k]
    for (int i = 0; i < n - k; i++) {
        // 将新右端点 i+k 加入最小值队列
        while (!minDQ.empty() && v[i + k] < v[minDQ.back()]) {
            minDQ.pop_back();
        }
        minDQ.push_back(i + k);
        // 将新右端点 i+k 加入最大值队列
        while (!maxDQ.empty() && v[i + k] > v[maxDQ.back()]) {
            maxDQ.pop_back();
        }
        maxDQ.push_back(i + k);

        // 下标不大于 i 的元素已经离开新窗口 [i+1, i+k]
        while (minDQ.front() <= i) minDQ.pop_front();
        while (maxDQ.front() <= i) maxDQ.pop_front();

        // 清理后两个队首分别为新窗口的最小值和最大值
        minRes.push_back(v[minDQ.front()]);
        maxRes.push_back(v[maxDQ.front()]);
    }

    // 按题目要求先输出所有窗口最小值，再输出所有窗口最大值
    for (auto i : minRes) cout << i << " ";
    cout << '\n';
    for (auto i : maxRes) cout << i << " ";
    return 0;
}
