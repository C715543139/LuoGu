/**
 * Luogu P2415 - 集合求和
 *
 * 问题描述：
 *   求集合 S 的所有子集中，全部元素之和的总和。
 *
 * 解法：计算每个元素的出现次数
 *   对任意元素 x，包含 x 的子集可以从其余 n - 1 个元素中任意选择，
 *   因此 x 恰好在 2^(n - 1) 个子集中出现。
 *   所有子集元素和为：
 *       (S 中全部元素之和) * 2^(n - 1)。
 *   代码先求全部元素之和，再连续乘 n - 1 次 2。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n)
 *   - 空间复杂度：O(n)，用于保存输入元素并取得集合大小
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 输入没有给出元素个数，读取至 EOF；v 用于记录元素数量 n。
    vector<int> v;
    int a;
    // 题目保证答案不超过 10^18，unsigned long long 可以安全保存。
    unsigned long long sum = 0;
    while (cin >> a) {
        // 先累计 S 中所有元素的和。
        sum += a;
        v.push_back(a);
    }

    int n = v.size();
    // 每个元素在 2^(n - 1) 个子集中出现，以下循环计算这个乘数。
    for (int i = 0; i < n - 1; i++) {
        sum *= 2;
    }

    // 输出所有子集元素和的总和。
    cout << sum << endl;
    return 0;
}
