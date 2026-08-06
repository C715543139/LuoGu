/**
 * Luogu P1115 - 最大子段和
 *
 * 问题描述：
 *   给定一个整数序列，选择一个连续且非空的子段，使其元素和最大。
 *
 * 解法：前缀和 + 维护最小前缀和
 *   1. 设 pre[i] 为前 i 个数的和，则区间 [l, r] 的和为：
 *        pre[r] - pre[l-1]。
 *   2. 枚举右端点 r。要使以 r 结尾的子段和最大，
 *      只需减去 r 之前最小的前缀和。
 *   3. 扫描过程中维护 minNum = min(pre[0], pre[1], ..., pre[r-1])，
 *      用 pre[r] - minNum 更新答案，再将 pre[r] 纳入最小前缀和的候选。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n)
 *   - 空间复杂度：O(n)，用于保存前缀和数组
 */

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int main() {
    int n;
    cin >> n;
    // v[i] 在读入后保存前缀和 pre[i]；v[0]=0 是空前缀的和
    vector v(n + 1, 0);
    // 读入数据并原地构造前缀和
    for (int i = 1; i <= n; i++) {
        cin >> v[i];
        v[i] += v[i - 1];
    }

    // sum：目前最大的非空子段和
    // minNum：当前右端点之前的最小前缀和，初始为 pre[0]=0
    long long sum = LONG_LONG_MIN, minNum = 0;
    // 枚举每个位置 i 作为子段右端点
    for (int i = 1; i <= n; i++) {
        // 以 i 结尾的最优子段和 = pre[i] - 之前最小的前缀和
        sum = max(sum, v[i] - minNum);
        // 在计算答案后更新，确保子段至少包含第 i 个元素
        minNum = min(minNum, (long long)v[i]);
    }
    // 输出最大子段和
    cout << sum << endl;
    return 0;
}
