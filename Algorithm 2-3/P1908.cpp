/**
 * Luogu P1908 - 逆序对
 *
 * 问题描述：
 *   统计满足 i < j 且 a[i] > a[j] 的数对数量。序列中允许出现重复值。
 *
 * 解法：PBDS 顺序统计树
 *   从左到右扫描序列，顺序统计树保存已经出现过的元素。
 *   每个元素以 {数值, 下标} 作为键：下标使重复数值也能作为不同元素存入树中。
 *   对当前 a[i]，order_of_key({a[i], i}) 统计此前小于等于 a[i] 的元素数；
 *   已插入元素总数减去该数量，便是此前严格大于 a[i] 的元素数，
 *   即以 i 为右端点新增的逆序对数量。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n log n)
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

// ordered_set 按 pair 的字典序排序，并支持第 k 小与键的排名查询。
using ordered_set = tree<std::pair<int, int>, null_type, std::less<>, rb_tree_tag,
    tree_order_statistics_node_update>;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector v(n, 0);
    for (int i = 0; i < n; ++i) std::cin >> v[i];

    long long count = 0; // 逆序对总数需要 long long 保存
    ordered_set os;
    for (int i = 0; i < n; i++) {
        // 之前插入的相等元素下标均小于 i，因此它们也小于 {v[i], i}。
        // 所以 order_of_key 的结果正好是此前小于等于 v[i] 的元素数。
        count += os.size() - os.order_of_key({v[i], i});

        // 下标保证键唯一，从而不会丢失重复数值。
        os.insert({v[i], i});
    }
    std::cout << count << '\n';
    return 0;
}
