/**
 * Luogu P4290 - [HAOI2008] 玩具取名
 *
 * 问题描述：
 *   初始名字是 W、I、N、G 中的一个字母。每条替换规则可以将一个字母替换为
 *   两个字母，替换过程可以重复进行。给定最终得到的字符串，按照 W、I、N、G
 *   的顺序输出它可能由哪些初始字母变形得到；如果均不可能，则输出指定提示。
 *
 * 解法：区间动态规划
 *   将替换过程反向考虑：如果字母 c 可以替换为 xy，并且某个区间能够由 x 生成，
 *   相邻的另一个区间能够由 y 生成，那么合并后的完整区间就能够由 c 生成。
 *
 *   mp[xy] 保存能够直接替换为字符串 xy 的所有父字母。
 *   设 dp[i][j] 为字符集合，表示子串 l[i...j] 可能由集合中的哪些字母生成。
 *
 *   边界状态：
 *     长度为 1 的子串 l[i...i] 可以由该字符本身生成，
 *     因此将 l[i] 放入 dp[i][i]。
 *
 *   状态转移：
 *     枚举区间 [i, j] 的分界点 k，将其分为 [i, k-1] 和 [k, j]。
 *     再分别枚举两个子区间可能生成的字母 x 和 y，查询 mp[xy]，
 *     将所有能够替换为 xy 的父字母加入 dp[i][j]。
 *     区间按照长度从小到大计算，转移所依赖的两个子区间会被提前求出。
 *
 * 复杂度分析：
 *   设最终字符串长度为 L。每个状态集合最多包含 4 个字母。
 *   - 时间复杂度：O(L^3 * 4^2)，通常记为 O(L^3)；
 *   - 空间复杂度：O(L^2)，但 unordered_set 会带来较大的额外空间开销。
 *
 * 可以优化的点：
 *   1. W、I、N、G 只有四种，可以用一个四位整数代替 unordered_set<char>，
 *      以按位或完成集合合并，减少哈希和动态内存分配开销。
 *   2. 两字符组合总共只有 16 种，可以用 rule[4][4] 的位掩码数组代替
 *      gp_hash_table<string, vector<char>>，同时避免在转移中反复构造字符串。
 *   3. mp[ss] 在规则不存在时会插入一个空项；若保留哈希表，可以改用 find 查询，
 *      避免 DP 过程中修改规则表。
 *   4. 可以预计算 combine[leftMask][rightMask]。转移时只需一次数组查询和按位或，
 *      将每个分界点处最多 16 次的字母组合检查压缩为一次操作。
 *   5. 当某个区间已经能够由全部四个字母生成时，其结果不会继续增加，
 *      可以提前结束该区间剩余分界点的枚举。
 */

#include <iostream>
#include <unordered_set>
#include <vector>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;

int main() {
    // 读入 W、I、N、G 四个字母各自拥有的替换规则数量
    int wn, in, nn, gn;
    cin >> wn >> in >> nn >> gn;

    // mp[xy] 保存所有能够替换为 xy 的父字母
    __gnu_pbds::gp_hash_table<string, vector<char>> mp;

    // 按照 W、I、N、G 的顺序读入每个父字母对应的规则
    for (auto &[c, n]: vector<pair<char, int>>
         {{'W', wn}, {'I', in}, {'N', nn}, {'G', gn}}) {
        for (int i = 0; i < n; i++) {
            string x;
            cin >> x;

            // 规则 c -> x 反向记录为：字符串 x 可以合并成父字母 c
            mp[x].push_back(c);
        }
    }

    // l 是经过若干次替换后得到的最终名字
    string l;
    cin >> l;
    int size = l.size();

    // dp[i][j] 保存子串 l[i...j] 可能由哪些字母生成
    vector dp(size, vector<unordered_set<char>>(size));

    // 单个字符无需进行替换，可以由其自身生成
    for (int i = 0; i < size; i++) {
        dp[i][i].insert(l[i]);
    }

    // 按区间长度从小到大计算，保证左右子区间已经求出
    for (int len = 2; len <= size; len++) {
        for (int i = 0; i <= size - len; i++) {
            int j = i + len - 1;

            // 在 k 处分割区间：[i, k-1] 和 [k, j]
            for (int k = i + 1; k <= j; k++) {
                // 枚举左右子区间可能生成的字母
                for (const char &x: dp[i][k - 1]) {
                    for (const char &y: dp[k][j]) {
                        // ss 表示左右两个父字母组成的替换结果 xy
                        string ss;
                        ss += x;
                        ss += y;

                        // 能够替换为 xy 的字母都可以生成当前完整区间
                        auto &vt = mp[ss];
                        dp[i][j].insert(vt.begin(), vt.end());
                    }
                }
            }
        }
    }

    // 完整字符串无法由任何一个基本字母生成
    if (dp[0][size - 1].empty()) {
        cout << "The name is wrong!" << endl;
        return 0;
    }

    // 按照题目要求的 W、I、N、G 顺序输出所有可能的初始字母
    auto &st = dp[0][size - 1];
    for (auto c: {'W', 'I', 'N', 'G'}) {
        if (st.count(c)) {
            cout << c;
        }
    }
    return 0;
}
