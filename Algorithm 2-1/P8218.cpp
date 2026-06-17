/**
 * Luogu P8218 - 【深进1.例1】求区间和
 *
 * 问题描述：
 *   给定一个长度为 m 的序列 a[1..m]，接下来有 n 次查询，
 *   每次查询给定区间端点 (p, q)，要求输出 a[p] + a[p+1] + ... + a[q]。
 *
 * 解法：一维前缀和
 *   1. 边读入边构建前缀和数组 v：
 *        v[i] = a[1] + a[2] + ... + a[i]
 *      递推式：v[i] = v[i-1] + a[i]
 *   2. 每次查询 [p, q] 的区间和利用前缀和 O(1) 求出：
 *        sum(p, q) = v[q] - v[p-1]
 *
 * 复杂度分析：
 *   - 时间复杂度：O(m + n)，预处理 O(m)，每次查询 O(1)
 *   - 空间复杂度：O(m)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int m;
    cin >> m;  // 序列长度
    // v 为前缀和数组（1-based），多开一格便于处理 v[0] = 0
    vector v(m + 1, 0);
    // 边读入边构建前缀和
    for (int i = 1, temp; i <= m; i++) {
        cin >> temp;
        v[i] = temp + v[i - 1];  // 递推：前缀和[i] = 前缀和[i-1] + a[i]
    }

    // 处理 n 次区间查询
    int n;
    cin >> n;  // 查询次数
    for (int i = 0, p, q; i < n; i++) {
        cin >> p >> q;
        // 区间 [p, q] 的元素和 = 前缀和[q] - 前缀和[p-1]
        cout << v[q] - v[p - 1] << endl;
    }
    return 0;
}
