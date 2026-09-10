/**
 * Luogu P1874 - 快速求和
 *
 * 问题描述：
 *   给定一个长度不超过 40 的数字字符串，在其中插入若干个加号，
 *   使分割得到的所有数字之和等于目标值 n，求最少需要插入多少个加号。
 *   每段数字允许包含前导 0；如果无法得到目标值，则输出 -1。
 *
 * 解法：动态规划
 *   设 dp[i][j] 表示使用字符串的前 i 个字符，得到数字之和 j 时，
 *   最少需要插入的加号数量。INT_MAX 表示该状态无法到达。
 *
 *   计算 dp[i][j] 时，枚举最后一段数字的起点 k：
 *     s[0...k-1] + s[k...i-1]
 *   如果 s[k...i-1] 表示的数值为 num，并且前 k 个字符能够组成 j-num，
 *   则可以在位置 k 前插入一个加号，得到转移：
 *     dp[i][j] = min(dp[i][j], dp[k][j-num] + 1)
 *
 *   当前 i 个字符也可能整体作为一个数字，此时不需要插入加号，
 *   对应初始化 dp[i][value(s[0...i-1])] = 0。
 *
 *   解析数字段时逐位计算。一旦数值超过 n，后续继续添加数字也不会使其减小，
 *   因此可以提前停止。这种方式既能处理任意长度的前导 0，也能避免整数越界。
 *
 * 复杂度分析：
 *   设字符串长度为 m。
 *   - 时间复杂度：O(m^2 * n + m^3)，其中 m <= 40；
 *   - 空间复杂度：O(m * n)。
 */

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int main() {
    // 读入数字字符串和目标数字
    string s;
    cin >> s;
    int n;
    cin >> n;

    int size = s.size();

    // dp[i][j] 表示使用前 i 个字符得到和 j 时，最少需要插入的加号数量
    vector dp(size + 1, vector(n + 1, INT_MAX));

    // 计算 s[left...right-1] 表示的数值。
    // 一旦数值超过目标 n，继续追加数字也不可能重新变小，直接返回 n + 1。
    // 逐位计算可以安全处理任意数量的前导 0，也避免 stoi 发生越界。
    auto getNumber = [&](int left, int right) {
        int value = 0;
        for (int pos = left; pos < right; ++pos) {
            value = value * 10 + (s[pos] - '0');
            if (value > n) {
                return n + 1;
            }
        }
        return value;
    };

    // 依次计算使用前 i 个字符能够得到的所有数字之和
    for (int i = 1; i <= size; i++) {
        // 将前 i 个字符整体作为一个数字，不需要插入加号。
        int ni = getNumber(0, i);
        if (ni <= n) {
            dp[i][ni] = 0;
        }

        // 枚举最后一段数字的起点 k，位置 k 前需要插入一个加号
        for (int k = 1; k <= i - 1; ++k) {
            // 在 k 前插入加号，最后一段数字为 s[k...i-1]。
            int num = getNumber(k, i);
            if (num > n) {
                continue;
            }

            // 枚举加入最后一段数字后得到的总和 j
            for (int j = num; j <= n; j++) {
                // 前 k 个字符能够得到 j-num 时，当前状态可以由它转移而来
                if (dp[k][j - num] != INT_MAX) {
                    dp[i][j] = min(dp[i][j], dp[k][j - num] + 1);
                }
            }
        }
    }
    // 输出组成目标值所需的最少加号数；无法组成时输出 -1
    cout << (dp[size][n] == INT_MAX ? -1 : dp[size][n]) << endl;
    return 0;
}
