/**
 * Luogu P3369 - 【模板】普通平衡树
 *
 * 问题描述：
 *   动态维护一个可重集合，支持插入、删除一个元素、查询排名、查询指定排名的值，
 *   以及查询一个数的严格前驱和严格后继。
 *
 * 解法：GNU PBDS 顺序统计树
 *   PBDS tree 与 set 一样要求键互不相同，因此使用 pair(value, id) 作为键：
 *   value 是题目中的数值，id 使用插入操作发生时的操作编号，保证每次插入的键唯一，
 *   从而允许多个 value 相同的元素同时存在。
 *
 *   pair 按字典序比较：先比较 value，只有 value 相等时才比较 id。因此整棵树首先
 *   按题目数值排序，相同数值再按唯一编号排序。INT_MIN 位于所有实际 id 之前，
 *   INT_MAX 位于所有实际 id 之后，可用于确定一组重复值的左右边界。
 *
 *   tree_order_statistics_node_update 提供两个顺序统计接口：
 *   - order_of_key(key)：返回严格小于 key 的元素数量；
 *   - find_by_order(k)：返回 0-based 排名为 k 的元素迭代器。
 *
 * 复杂度分析：
 *   - 每次插入、删除、排名、指定排名、前驱和后继查询均为 O(log n)
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <climits>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;

int main() {
    // 红黑树保存 pair(value, id)，null_type 表示只保存键而不附加映射值。
    // tree_order_statistics_node_update 使其支持 order_of_key 和 find_by_order。
    __gnu_pbds::tree<pair<int, int>, __gnu_pbds::null_type, less<>,
        __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> ordered_set;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int opt, x;
        cin >> opt >> x;
        switch (opt) {
            case 1:
                // i 是当前操作编号且不会重复，使相同的 x 也能以不同 pair 插入树中。
                ordered_set.insert(pair(x, i));
                break;
            case 2: {
                // (x, INT_MIN) 位于所有 (x, id) 之前，lower_bound 找到第一个 x。
                auto it = ordered_set.lower_bound(pair(x, INT_MIN));
                if (it != ordered_set.end() && it->first == x) {
                    // 只删除一个迭代器所指元素，其他重复的 x 仍然保留。
                    ordered_set.erase(it);
                }
                break;
            }
            case 3:
                // 严格小于 (x, INT_MIN) 的键正好对应所有数值小于 x 的元素。
                // 题目规定的排名从 1 开始，因此在数量上加 1。
                cout << ordered_set.order_of_key(pair(x, INT_MIN)) + 1 << '\n';
                break;
            case 4:
                // find_by_order 使用 0-based 下标，题目给出的排名 x 使用 1-based。
                cout << ordered_set.find_by_order(x - 1)->first << '\n';
                break;
            case 5:
                // 第一个不小于 x 的元素之前一个元素，就是严格小于 x 的最大值。
                // 题目保证前驱存在，因此递减迭代器是安全的。
                cout << (--ordered_set.lower_bound(pair(x, INT_MIN)))->first << '\n';
                break;
            case 6:
                // (x, INT_MAX) 排在所有重复的 x 之后；upper_bound 得到严格大于 x 的最小值。
                // 题目保证后继存在，因此结果不会是 end()。
                cout << ordered_set.upper_bound(pair(x, INT_MAX))->first << '\n';
                break;
            default: ;
        }
    }
    return 0;
}
