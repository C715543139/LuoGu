/**
 * Luogu P1091 - [NOIP 2004 提高组] 合唱队形
 *
 * 问题描述：
 *   从一排同学中出列若干人，使剩下的同学身高先严格上升，再严格下降，
 *   求最少出列人数。
 *
 * 解法：贪心 + 二分查找
 *   分别从左到右和从右到左处理身高序列，求每个位置作为最高点时，
 *   左侧和右侧分别至少需要出列多少人。
 *
 *   l 和 r 维护当前方向下各长度子序列的最小末尾身高：
 *   - 从左到右处理时，维护严格上升子序列；
 *   - 从右到左处理时，维护严格下降子序列。
 *
 *   对当前身高使用 lower_bound 查找第一个大于等于它的位置：
 *   - 如果查找位置在末尾，说明当前身高可以扩展最长子序列；
 *   - 否则用当前身高替换该位置，降低相同长度子序列的末尾身高。
 *
 *   res[i] 表示以第 i 位同学作为最高点时需要出列的人数。
 *   枚举每个位置作为最高点，取最小的出列人数。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n log n)
 *   - 空间复杂度：O(n)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 读入同学人数和每位同学的身高
    int n;
    cin >> n;
    vector v(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    // minRes 保存目前找到的最少出列人数
    int minRes = 100;

    // l、r 分别维护从左向右和从右向左处理时的序列状态
    // res[i] 保存以第 i 位同学作为最高点时的出列人数
    vector<int> l, r, res(n, 0);

    // 从左到右处理，计算每个位置左侧需要出列的人数
    for (int i = 0; i < n; i++) {
        // 查找第一个大于等于当前身高的位置，保证序列严格上升
        auto lIt = lower_bound(l.begin(), l.end(), v[i]);
        int lRes;

        // 当前身高可以扩展已有的最长严格上升子序列
        if (lIt == l.end()) {
            l.push_back(v[i]);
            lRes = i + 1 - l.size();
        } else {
            // 替换相同长度序列的末尾，保留后续扩展空间
            *lIt = v[i];
            lRes = i - (lIt - l.begin());
        }

        // 记录当前同学左侧需要出列的人数
        res[i] += lRes;
    }

    // 从右到左处理，计算每个位置右侧需要出列的人数
    for (int i = 0; i < n; i++) {
        // 逆序处理后，可以用同样的方法维护严格下降序列
        auto rIt = lower_bound(r.begin(), r.end(), v[n - 1 - i]);
        int rRes;

        // 当前身高可以扩展已有序列
        if (rIt == r.end()) {
            r.push_back(v[n - 1 - i]);
            rRes = i + 1 - r.size();
        } else {
            // 替换相同长度序列的末尾，保留后续扩展空间
            *rIt = v[n - 1 - i];
            rRes = i - (rIt - r.begin());
        }

        // 合并当前最高点两侧的出列人数，并更新最优答案
        res[n - 1 - i] += rRes;
        minRes = min(minRes, res[n - 1 - i]);
    }

    // 输出最少需要出列的人数
    cout << minRes << endl;
    return 0;
}
