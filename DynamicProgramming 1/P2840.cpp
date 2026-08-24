/**
 * Luogu P2840 - 纸币问题 2
 *
 * 问题描述：
 *   有 n 种面额互不相同、数量无限的纸币，求支付金额 w 的方案数。纸币的支付顺序
 *   不同视为不同方案，答案对 10^9+7 取模。
 *
 * 解法：记忆化搜索
 *   设 solve(cur) 表示支付金额 cur 的有序方案数。支付 0 元时已经构成一种完整方案，
 *   所以 solve(0)=1。对于每种面额 a，若 cur>=a，就把 a 作为当前序列的第一张纸币，
 *   后续可按 solve(cur-a) 种方式支付剩余金额。因此：
 *       solve(cur) = sum(solve(cur-a))，其中 a<=cur
 *
 *   每一层递归都选择当前序列的下一张纸币，同一组纸币的不同选择顺序会产生不同的
 *   递归路径，因而会被分别计数。ht 缓存已经计算过的金额，避免重复计算。
 *
 * 复杂度分析：
 *   每个金额至多计算一次，每次枚举 n 种纸币：
 *   - 时间复杂度：O(nw)，哈希表操作按均摊 O(1) 计算
 *   - 空间复杂度：O(w)，递归栈最深也为 O(w)
 */

#include <iostream>
#include <vector>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;

// v 保存每种纸币的面额；ht 缓存每个金额对应的有序方案数。
vector<int> v;
__gnu_pbds::gp_hash_table<int, int> ht;

// 返回支付 cur 元的有序方案数。
int solve(int cur) {
    if (cur == 0) {
        // 当前序列已经恰好支付完，计作一种方案。
        return 1;
    }

    if (ht.find(cur) != ht.end()) {
        // 当前金额已经计算过，直接复用其方案数。
        return ht[cur];
    }

    // 枚举当前序列的下一张纸币，累加各种选择对应的方案数。
    int res = 0;
    for (int a: v) {
        if (cur >= a) {
            // 先支付 a 元，剩余 cur-a 元的每种有序方案都能接在 a 后面。
            res = (solve(cur - a) + res) % 1000000007;
        }
    }
    // 缓存金额 cur 的方案数，避免不同递归路径重复求解。
    ht[cur] = res;
    return res;
}

int main() {
    int n, w;
    cin >> n >> w;
    // 读入 n 种纸币面额。
    v.assign(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }
    // 输出支付目标金额 w 的有序方案数。
    cout << solve(w) << endl;
    return 0;
}
