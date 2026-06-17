/**
 * Luogu P2367 - 语文成绩
 *
 * 问题描述：
 *   给定一个长度为 n 的序列，以及 p 次区间操作（每次将区间 [x, y] 的元素增加 z），
 *   求所有操作完成后序列中的最小值。
 *
 * 解法一 (solve)：暴力 + 优化
 *   - 直接对每次操作遍历区间累加 z
 *   - 当区间长度大于 n/2 时，反向处理：累加 z 到全局 add，并对区间外的两端做减法
 *   - 时间复杂度：取决于区间长度，特殊情况下较优
 *
 * 解法二 (solve2)：差分数组
 *   - 将原数组转成差分数组：d[i] = a[i] - a[i-1]
 *   - 每次区间操作 [x, y] 只需修改两端：
 *       d[x-1] += z
 *       d[y]   -= z （若 y < n）
 *   - 最后通过前缀和还原原数组，并扫描求最小值
 *   - 时间复杂度：O(n + p)，相比解法一效率显著提升
 *
 * 复杂度分析（solve2）：
 *   - 时间复杂度：O(n + p)
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <vector>
using namespace std;

/**
 * 解法一：暴力 + 区间长度优化
 *
 * 思路：
 *   - 当区间为 [1, n] 时，所有元素都加 z，直接累加到 add 即可
 *   - 当区间长度 > n/2 时，反向处理：将 z 加到 add，并对两端之外的元素减 z
 *   - 其他情况直接遍历区间累加
 */
void solve() {
    int n, p;
    cin >> n >> p;  // n: 序列长度, p: 操作数量
    vector v(n, 0);

    int add = 0;  // 全局增量（用于优化全区间加法）
    // 读入初始序列
    for (int i = 0; i < n; i++) cin >> v[i];
    // 处理 p 次区间操作
    for (int i = 0; i < p; i++) {
        int x, y, z;
        cin >> x >> y >> z;
        if (x == 1 && y == n) {
            // 区间为整个序列，累加到 add
            add += z;
        } else if (y - x > n / 2) {
            // 区间长度 > n/2，反向处理
            add += z;
            // 对区间左端之前的元素减 z
            for (int j = 0; j < x - 1; j++) v[j] -= z;
            // 对区间右端之后的元素减 z
            for (int j = y; j < n; j++) v[j] -= z;
        } else {
            // 一般情况：直接遍历区间加 z
            for (int j = x - 1; j < y; j++) v[j] += z;
        }
    }

    // 求最小值（注意要加上全局增量 add）
    int res = 2147483647;
    for (int num: v) res = min(res, num);
    cout << res + add << endl;
}

/**
 * 解法二：差分数组
 *
 * 思路：
 *   - 差分是前缀和的逆过程：
 *       原数组 a 与差分数组 d 满足：a[i] = d[0] + d[1] + ... + d[i]
 *   - 区间修改 [x, y] 加 z 等价于：
 *       d[x-1] += z
 *       d[y]   -= z （若 y < n，保证不影响后续前缀和）
 *   - 通过一次前缀和即可还原 a，并扫描求最小值
 */
void solve2() {
    int n, p;
    cin >> n >> p;  // n: 序列长度, p: 操作数量
    vector v(n, 0);

    // 读入原序列时直接转为差分数组
    cin >> v[0];
    for (int i = 1, last = v[0], cur; i < n; i++) { // 差合，前缀和的逆原理
        cin >> cur;
        v[i] = cur - last;  // 差分：d[i] = a[i] - a[i-1]
        last = cur;
    }
    // 处理 p 次区间操作（差分修改只需 O(1)）
    for (int i = 0; i < p; i++) {
        int x, y, z;
        cin >> x >> y >> z;

        v[x - 1] += z;        // 区间左端点 +z
        if (y < n) v[y] -= z; // 区间右端点 +1 处 -z（避免越界）
    }

    // 通过前缀和还原原数组，并扫描求最小值
    int num = v[0], res = num;
    for (int i = 1; i < n; i++) {
        num += v[i];          // 前缀和累加，还原 a[i]
        res = min(res, num);
    }
    cout << res << endl;
}

int main() {
    solve2();  // 使用差分解法（更优）
    return 0;
}
