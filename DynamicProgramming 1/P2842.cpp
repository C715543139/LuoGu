/**
 * Luogu P2842 - 纸币问题 1
 *
 * 问题描述：
 *   有 n 种面额的纸币，每种纸币都可以无限使用。求恰好凑出金额 w 所需的最少纸币
 *   张数，题目保证 w 一定可以凑出。
 *
 * 解法：记忆化搜索
 *   设 solve(cur) 表示凑出金额 cur 所需的最少纸币张数。凑出 0 元不需要纸币，
 *   因此 solve(0)=0。对于每种面额 A，若 cur>=A，就可以先使用一张 A 元纸币，
 *   再凑出 cur-A 元，即：
 *       solve(cur) = min(solve(cur-A)+1)
 *
 *   memory 记录已经计算过的金额，避免重复递归。同一个金额最多计算一次，每次枚举
 *   所有 n 种纸币。不能凑出的中间金额保持为 10001；由于 w<=10^4，这个值可作为
 *   无穷大使用，且不会影响最终可行答案。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(nw)
 *   - 空间复杂度：O(w)，递归栈最深也为 O(w)
 */

#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

// v 保存每种纸币的面额；memory[cur] 保存 solve(cur) 的计算结果。
vector<int> v;
unordered_map<int, int> memory;

// 返回凑出 cur 元所需的最少纸币张数。
int solve(int cur) {
    if (cur == 0) {
        // 金额恰好凑完，不需要再使用纸币。
        return 0;
    }
    if (memory[cur]) {
        // 已计算过当前金额，直接复用结果。
        return memory[cur];
    }

    // 10001 大于任意可行答案，可表示当前金额暂时无法凑出。
    int res = 10001;
    for (int w: v) {
        if (cur >= w) {
            // 先使用一张 w 元纸币，再递归凑出剩余的 cur-w 元。
            res = min(res, solve(cur - w) + 1);
        }
    }
    // 记录当前金额的最优答案，供之后的递归状态复用。
    memory[cur] = res;
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
    // solve(w) 就是凑出目标金额 w 的最少纸币张数。
    cout << solve(w) << endl;
    return 0;
}
