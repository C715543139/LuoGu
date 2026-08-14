/**
 * Luogu P2085 - 最小函数值
 *
 * 问题描述：
 *   给定 n 个二次函数 F_i(x)=A_i*x^2+B_i*x+C_i，其中 x 为正整数。
 *   将所有函数能够产生的函数值放在一起，输出其中最小的 m 个，重复值也要保留。
 *
 * 解法：枚举候选值 + 固定容量大根堆
 *   题目保证 A_i>0、B_i>=0。对于 x>=1：
 *     F_i(x+1)-F_i(x)=A_i*(2x+1)+B_i>0，
 *   因此每个函数产生的值都随 x 严格递增。
 *
 *   对于任意一个函数，只需考虑它的前 m 个函数值。因为第 m+1 个及之后的值，
 *   前面已经存在至少 m 个更小的同函数值，所以它不可能进入全局最小的 m 个值。
 *
 *   使用大根堆 pq 保存当前找到的最小 m 个函数值：
 *   1. 先用第一个函数的前 m 个值填满堆；
 *   2. 对后续函数按 x 递增枚举函数值；
 *   3. 若当前值小于 pq.top()，就替换堆中最大的候选值；
 *   4. 若当前值不小于 pq.top()，由于后续函数值只会更大，可以停止枚举该函数。
 *
 *   priority_queue 可以保存重复元素。当前值等于堆顶时即使不替换，堆中保留的
 *   数值序列也不会改变，因此不会影响重复值的输出结果。
 *
 * 复杂度分析：
 *   - 最坏枚举 O(nm) 个候选值，每次更新堆需要 O(log m)
 *   - 最坏时间复杂度：O(nm log m)，实际枚举常会因提前终止而减少
 *   - 空间复杂度：O(m)
 */

#include <cmath>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    // 大根堆中始终保留当前最小的 m 个值，堆顶是其中最大的一个，即筛选阈值。
    priority_queue<int> pq;
    for (int i = 0; i < n; i++) {
        int a, b, c;
        cin >> a >> b >> c;

        // 从抛物线顶点右侧的第一个正整数开始枚举。
        // 本题 b>=0，顶点 -b/(2a)<=0，因此 start 实际恒为 1。
        int start = max(1, (int)ceil(1.0 * -b / 2 / a));

        // 单个函数最多枚举前 m 个值，再往后的值一定无法进入全局前 m 小。
        for (int j = 0; j < m; j++) {
            int res = a * start * start + b * start + c;

            // 第一个函数用于填满堆，建立最初的 m 个候选值。
            if (i == 0) {
                pq.push(res);
            } else if (res < pq.top()) {
                // 用更小的函数值替换当前候选集合中的最大值。
                pq.pop();
                pq.push(res);
            } else {
                // 当前函数值已达到筛选阈值，后续递增的函数值也不可能入选。
                break;
            }
            start++;
        }
    }

    // 大根堆按从大到小弹出；倒序写入数组后即可得到升序答案。
    vector<int> v(m);
    for (int i = 0; i < m; i++) {
        v[m - i - 1] = pq.top();
        pq.pop();
    }

    for (int num: v) cout << num << ' ';
    return 0;
}
