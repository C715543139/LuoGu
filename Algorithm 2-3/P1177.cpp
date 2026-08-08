/**
 * Luogu P1177 - 【模板】排序
 *
 * 问题描述：
 *   给定 n 个整数，按从小到大的顺序输出整个序列。
 *
 * 解法：调用标准库 std::sort
 *   std::sort 直接对 vector 中的全部元素进行升序排序。
 *
 * 复杂度分析：
 *   - 排序：O(n log n)
 *   - 空间复杂度：O(n)，用于保存输入序列
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;


int main() {
    // 关闭与 C 标准输入输出的同步，并解除 cin 与 cout 的绑定，加快大量数据的读写
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // v 保存输入的 n 个待排序整数
    vector v(n, 0);
    for (int i = 0; i < n; i++) cin >> v[i];

    // 默认比较规则为从小到大
    sort(v.begin(), v.end());

    // 依次输出排序后的序列；行末多出的空格不影响评测结果
    for (int i = 0; i < n; i++) cout << v[i] << ' ';
    return 0;
}
