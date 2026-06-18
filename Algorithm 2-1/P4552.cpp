/**
 * Luogu P4552 - [Poetize6] IncDec Sequence（增减序列）
 *
 * 问题描述：
 *   给定一个长度为 n 的整数序列 a[1..n]，可以执行任意次如下操作：
 *     选择区间 [l, r]（1 ≤ l ≤ r ≤ n），将该区间内每个元素加 1 或减 1。
 *   现要求：
 *     1) 最少需要多少次操作，能使整个序列中所有元素相等；
 *     2) 在最少操作次数下，最终可以得到的"目标值"共有多少种可能。
 *
 * 解法：差分数组 + 正负配对
 *   1) 把原序列 a 转成差分数组 d：
 *        d[1] = a[1]
 *        d[i] = a[i] - a[i-1]   (2 ≤ i ≤ n)
 *      由于区间加减 [l, r] 等价于差分上"两端点"修改，因此问题转化为：
 *      通过最少的"d[某位] += 1, d[某位] -= 1"操作，使 d[2..n] 全部为 0，
 *      d[1] 的最终值（设为 t）即为序列最终的目标值。
 *
 *   2) 设 up = d[2..n] 中所有正数之和，low = d[2..n] 中所有负数之和。
 *      每次操作可以让一个正数与一个负数"配对"抵消（d[x] -= 1, d[y] += 1），
 *      配对数 = min(up, -low)；剩余的多出部分只能与 d[1] 或 d[n+1] 配对，
 *      而 d[1] / d[n+1] 的累加结果即为"目标值 t"的调整。
 *      因此：
 *        - 最少操作次数 = max(up, -low) = up + low + max(up, -low) 的简化形式
 *        - 可选的 t 的范围由 up 与 -low 共同决定：
 *            t 共有 |up + low| + 1 种可能
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n)
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    long long n, up = 0, low = 0;  // n: 序列长度；up: 差分正数和；low: 差分负数和
    cin >> n;
    // v: 差分数组，v[0] 存 a[0] 自身，v[i] 存 a[i] - a[i-1]（i ≥ 1）
    vector<long long> v(n, 0);
    // last: 上一轮累加得到的 a[i-1] 值，用于求 a[i] - a[i-1]
    for (long long i = 0, last = 0; i < n; i++) {
        cin >> v[i];
        v[i] -= last;   // 原地转成差分：v[i] = a[i] - a[i-1]
        last += v[i];   // 累加 a[i]，作为下一轮的 a[i-1]
        if (i > 0) {
            // 仅对 i ≥ 1 的差分位统计正负和（d[1] 不参与配对）
            if (v[i] > 0) up += v[i];   // 正差分累加到 up
            else low += v[i];           // 负差分累加到 low
        }
    }

    // count: 最少操作次数 = max(up, -low)
    //   当 up > -low 时，所有负数被配对完后，正数还剩 (up + low)
    //   剩下的正数只能逐个与 d[1]/d[n+1] 配对 → (up + low) 次
    //   再加上内部配对消耗的 -low 次 → (up + low) + (-low) = up
    //   同理另一种情况得到 -low
    long long count = 0;
    if (up > -low) {
        count = (up + low) + (-low);
    } else {
        count = -(up + low) + up;
    }
    // 输出：最少操作次数，以及最少操作下可达到的目标值种类数
    // 目标值 t 的可选范围 = |up + low| + 1
    cout << count << endl << abs(up + low) + 1 << endl;
    return 0;
}
