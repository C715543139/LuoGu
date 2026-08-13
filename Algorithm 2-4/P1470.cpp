/**
 * Luogu P1470 - [IOI 1996 / USACO2.3] 最长前缀 Longest Prefix
 *
 * 问题描述：
 *   给定一个元素集合 P 和一个大写字母序列 S。集合中的元素可以重复使用，求 S 的
 *   最长前缀，使该前缀能够由 P 中若干个元素依次拼接得到。
 *
 * 解法：动态规划 + 哈希表
 *   dp[j] 表示 S 的前 j 个字符能否由集合中的元素拼成，空串可以被拼成，因此
 *   dp[0] = true。
 *
 *   若存在某种元素长度 len，满足：
 *   1. 前 j-len 个字符可以被拼成，即 dp[j-len] 为 true；
 *   2. S 中以位置 j 结尾、长度为 len 的子串属于元素集合 P；
 *   那么在该可分解前缀后再接上这个元素，便可得到 dp[j] = true。
 *
 *   使用 gp_hash_table 保存所有元素，以便快速判断一个子串是否属于 P；使用 set
 *   保存实际出现过的元素长度，转移时只需枚举这些长度。按 j 从小到大更新，并记录
 *   最后一个可达位置，即为最长可分解前缀的长度。
 *
 * 复杂度分析：
 *   设 |S| = n，不同元素长度的数量为 d，元素最大长度为 L。
 *   - 期望时间复杂度：O(n * d * L)，其中包含构造子串和计算哈希的开销；本题 L <= 10
 *   - 空间复杂度：O(n + sum(|p|))，p 为集合 P 中的元素
 */

#include <iostream>
#include <vector>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;

int main() {
    // 先按空白符读入全部字符串：换行不影响元素集合的读取，也不属于后续序列 S。
    vector<string> v;
    string temp;
    while (cin >> temp) {
        v.push_back(temp);
    }

    // sSet 保存元素本身，用于快速查询；lSet 去重并有序保存所有可能的元素长度。
    __gnu_pbds::gp_hash_table<string, __gnu_pbds::null_type> sSet;
    set<size_t> lSet;
    int i = 0;
    for (; i < v.size(); i++) {
        // 单独的 "." 标志着元素集合结束，跳过它后继续处理序列 S。
        if (v[i] == ".") {
            i++;
            break;
        }
        sSet.insert(v[i]);
        lSet.insert(v[i].length());
    }

    // S 可能分布在多行中；此前按空白符读入，因此将剩余片段直接连接起来。
    string s;
    for (; i < v.size(); i++) s += v[i];

    // dp[j] 表示长度为 j 的前缀 s[0..j-1] 能否由集合中的元素拼成。
    vector dp(s.length() + 1, false);
    // 空前缀无需使用任何元素，天然可达，是所有状态转移的起点。
    dp[0] = true;
    int res = 0;
    for (int j = 1; j <= s.length(); j++) {
        // 枚举元素集合中实际存在的长度；长度超过 j 时无法作为当前前缀的末段。
        for (auto it = lSet.begin(); it != lSet.end() && *it <= j; ++it) {
            // 若前半段可达，且末尾长度为 *it 的子串是一个元素，则整个前缀可达。
            if (dp[j - *it] && sSet.find(s.substr(j - *it, *it)) != sSet.end()) {
                dp[j] = true;
                // j 按递增顺序遍历，因此每次更新都会得到当前最大的可达前缀长度。
                res = j;
                // 已经证明 dp[j] 为 true，无需继续尝试其他元素长度。
                break;
            }
        }
    }
    cout << res << endl;
    return 0;
}
