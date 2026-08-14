/**
 * Luogu P1801 - 黑匣子
 *
 * 问题描述：
 *   按顺序向 Black Box 中加入 m 个整数。第 u[i] 个整数加入后执行第 i+1 次 GET，
 *   输出当前所有元素中的第 i+1 小值。u 单调不降，因此同一次插入后可能连续执行
 *   多次 GET。
 *
 * 解法：PBDS 顺序统计树
 *   使用带 tree_order_statistics_node_update 的红黑树动态维护所有已加入元素。
 *   find_by_order(k) 能在 O(log m) 时间内找到按升序排列后 0-based 的第 k 个元素，
 *   因而第 i+1 次 GET 直接查询 find_by_order(i)。
 *
 *   PBDS tree 和 set 一样不保存重复键，所以将每个元素存为 (数值, 插入下标)：
 *   插入下标使相同数值对应的键仍然互不相同；pair 按数值优先、下标次优先排序，
 *   因此查询结果的 first 仍是正确的第 k 小数值。
 *
 *   按 idx 从左到右插入 a。插入 a[idx] 后，Black Box 中恰有 idx+1 个元素；
 *   若 idx+1 等于下一次 GET 的 u[i]，便输出当前第 i+1 小值。while 能处理多个
 *   相同的 u[i]。
 *
 * 复杂度分析：
 *   - 每次插入和每次 find_by_order：O(log m)
 *   - 总时间复杂度：O((m + n) log m)
 *   - 空间复杂度：O(m)
 */

#include <iostream>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;

int main() {
    int m, n;
    cin >> m >> n;
    // add 保存依次加入的 a；get 保存每次 GET 对应的已插入元素数量 u。
    vector add(m, 0), get(n, 0);
    for (int i = 0; i < m; i++) cin >> add[i];
    for (int i = 0; i < n; i++) cin >> get[i];

    // 按 (数值, 插入下标) 升序维护元素，并支持按排名查找。
    // 插入下标用于区分数值相同的元素，避免 tree 对重复键去重。
    __gnu_pbds::tree<pair<int, int>, __gnu_pbds::null_type, less<>,
    __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> blackBox;
    // i 表示已经处理的 GET 数量，也是下一次 find_by_order 使用的 0-based 排名。
    int i = 0;
    for (int idx = 0; idx < m; idx++) {
        // 插入后 Black Box 中已有 idx+1 个元素。
        blackBox.insert({add[idx], idx});

        // u 单调不降；同一次插入后可能需要连续回答多次 GET。
        while (i < get.size() && idx + 1 == get[i]) {
            // 第 i+1 次 GET 要求第 i+1 小，对应 0-based 的第 i 个元素。
            cout << blackBox.find_by_order(i)->first << '\n';
            i++;
        }
    }
    return 0;
}
