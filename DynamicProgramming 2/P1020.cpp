/**
 * Luogu P1020 - [NOIP 1999 提高组] 导弹拦截
 *
 * 问题描述：
 *   某国研发了一套导弹拦截系统，系统拦截的第一发导弹可以到达任意高度，以后每一发
 *   导弹都不能高于前一发的高度。给定依次飞来的各导弹高度，求：
 *   1. 一套系统最多能拦截多少枚导弹（即最长不上升子序列长度）；
 *   2. 拦截所有导弹最少需要配备多少套这种导弹拦截系统。
 *
 * 解法：贪心 + 二分查找（Dilworth 定理）
 *   1. 第一问：求序列的最长不上升（非严格递减）子序列长度。
 *      将高度取相反数 x = -h，则等价于求 -h 的最长不下降（非严格递增）子序列。
 *      维护 tails1 数组，tails1[len-1] 表示长度为 len 的不降子序列末尾元素的最小值。
 *      对于新元素 x，使用 upper_bound 在 tails1 中查找第一个严格大于 x 的位置 it1：
 *      - 若 it1 == tails1.end()，说明 x >= 当前所有末尾元素，可直接接在末尾，长度加 1；
 *      - 否则更新 *it1 = x，在保持序列合法的同时减小末尾元素，为后续元素提供更大扩展空间。
 *
 *   2. 第二问：求将序列划分为最少个不上升子序列的划分数。
 *      根据 Dilworth 定理（偏序集的最少反链划分数等于最长链长度），最少不上升子序列
 *      划分数等于该序列的最长严格上升子序列（LIS）长度。
 *      维护 tails2 数组，tails2[len-1] 表示长度为 len 的严格上升子序列末尾元素的最小值。
 *      对于新高度 h，使用 lower_bound 在 tails2 中查找第一个大于等于 h 的位置 it2：
 *      - 若 it2 == tails2.end()，说明 h 严格大于当前所有末尾元素，可直接扩展，长度加 1；
 *      - 否则更新 *it2 = h，以更小的值替代，优化末尾元素。
 *
 * 复杂度分析：
 *   设导弹总数为 n：
 *   - 时间复杂度：O(n log n)，每枚导弹在 tails1 和 tails2 中各进行一次二分查找
 *   - 空间复杂度：O(n)，tails1 和 tails2 数组最多存储 n 个元素
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 优化输入输出效率
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // tails1[i] 保存长度为 i+1 的 -h 不降子序列末尾元素的最小值（第一问）
    // tails2[i] 保存长度为 i+1 的 h 严格上升子序列末尾元素的最小值（第二问）
    vector<int> tails1, tails2;

    int h;
    while (cin >> h) {
        // 第一问：h 的不升子序列 <=> -h 的不降子序列
        // upper_bound 查找第一个严格大于 -h 的位置，相等时仍可接在后面延伸
        int x = -h;
        auto it1 = upper_bound(tails1.begin(), tails1.end(), x);
        if (it1 == tails1.end()) {
            tails1.push_back(x);
        } else {
            *it1 = x;
        }

        // 第二问：根据 Dilworth 定理，最少系统数等价于最长严格上升子序列长度
        // lower_bound 查找第一个大于等于 h 的位置，相等时不能延伸严格上升序列
        auto it2 = lower_bound(tails2.begin(), tails2.end(), h);
        if (it2 == tails2.end()) {
            tails2.push_back(h);
        } else {
            *it2 = h;
        }
    }

    // 分别输出第一问和第二问的结果
    cout << tails1.size() << endl << tails2.size() << endl;
    return 0;
}