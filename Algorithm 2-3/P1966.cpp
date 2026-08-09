/**
 * Luogu P1966 - [NOIP 2013 提高组] 火柴排队
 *
 * 问题描述：
 *   分别交换两列中相邻火柴的位置，使两列对应位置的高度差平方和最小；
 *   在所有达到最小距离的方案中，求相邻交换次数的最小值。
 *
 * 解法：同秩配对 + 逆序对
 *   距离最小时，两列中第 i 小的火柴必须互相配对。
 *   本实现先按高度排序两列，将 v 中第 i 小火柴的高度替换为 t 中第 i 小的高度，
 *   再按原下标还原。此时 v[i].first 表示原位置 i 应放置的第二列火柴高度。
 *   扫描第二列原顺序时，将每根火柴映射到它应到达的目标位置；
 *   该位置排列的逆序对数量，就是使对应火柴相邻交换到正确顺序的最少次数。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n log n)
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // first 保存高度，second 保存读入时的 0-based 原位置。
    std::vector v(n, std::pair(0, 0)), t(n, std::pair(0, 0));
    for (int i = 0; i < n; ++i) {
        std::cin >> v[i].first;
        v[i].second = i;
    }
    for (int i = 0; i < n; ++i) {
        std::cin >> t[i].first;
        t[i].second = i;
    }

    // 按高度升序排列；排序后的相同下标 i 表示两列中第 i 小的火柴。
    std::sort(v.begin(), v.end());
    std::sort(t.begin(), t.end());
    for (int i = 0; i < n; ++i) {
        // 原 v[i] 位置上的火柴，应与 t[i] 位置上的火柴配对。
        // 保留 v[i].second，使这项对应关系仍绑定到第一列的原位置。
        v[i].first = t[i].first;
    }

    // 按原位置还原两列。第一列与第二列的数值上相等。
    std::sort(v.begin(), v.end(), [](std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; });
    std::sort(t.begin(), t.end(), [](std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; });

    // 目标是将第二列变换为第一列，选取第一列作为基准，将高度映射为升序下标 i。
    gp_hash_table<int, int> ht;
    for (int i = 0; i < n; ++i) {
        ht[v[i].first] = i;
    }

    // 求第二列的逆序对，即可变换为第一列。
    int count = 0;
    tree<int, null_type, std::less<>, rb_tree_tag, tree_order_statistics_node_update> os;
    for (int i = 0; i < n; ++i) {
        // 当前第二列火柴应移动到 pos；序列 pos 构成一个排列。
        int pos = ht[t[i].first];
        os.insert(pos);

        // 插入后，size - order_of_key(pos) - 1 是此前严格大于 pos 的目标位置数。
        // 每个这样的元素都与当前元素构成一个逆序对，需要一次相邻交换使二者交叉。
        count += os.size() - os.order_of_key(pos) - 1;
        count %= 100000000 - 3;
    }
    std::cout << count << std::endl;
    return 0;
}
