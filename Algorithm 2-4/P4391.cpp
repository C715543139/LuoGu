/**
 * Luogu P4391 - [BalticOI 2009] Radio Transmission 无线传输
 *
 * 问题描述：
 *   给定从循环播放的信息中连续接收到的字符串 s，求原信息可能具有的最短长度。
 *
 * 周期判定：
 *   长度 p 合法，当且仅当 s[p..l) 与 s[0..l-p) 完全相同；等价地，对所有
 *   p <= i < l，都有 s[i] == s[i-p]。接收可以在一个周期尚未结束时停止，
 *   因此无需满足 l 能被 p 整除。
 *
 * 解法：枚举周期长度
 *   solve1 从小到大枚举 p，并逐字符验证周期条件，首次成功的 p 即为最短长度。
 *   solve2 预处理字符串的多项式前缀哈希，使两个等长子串能够在 O(1) 时间内比较，
 *   再从小到大枚举 p。size_t 的无符号溢出相当于自动对 2^w 取模，其中 w 为
 *   size_t 的位数；哈希相等存在极低的碰撞可能。
 *
 * 复杂度分析：
 *   - solve1：最坏时间复杂度 O(l^2)，额外空间复杂度 O(1)
 *   - solve2：时间复杂度 O(l)，空间复杂度 O(l)
 *
 * 代码说明：
 *   main 实际调用使用前缀哈希的 solve2。
 */

#include <iostream>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;

// 直接比较版本：枚举每个候选周期，并检查相距 p 的字符是否全部相同。
void solve1() {
    int l;
    string s;
    cin >> l >> s;
    // 按长度递增枚举，首个合法的 p 就是答案；p == l 时一定合法。
    for (int p = 1; p <= l; p++) {
        bool ok = true;

        // 比较 s[p..l) 与同长度的前缀 s[0..l-p)。
        for (int i = p; i < l; i++) {
            if (s[i] != s[i - p]) {
                ok = false;
                break;
            }
        }

        if (ok) {
            cout << p << '\n';
            break;
        }
    }
}

// 前缀哈希版本的入口。
void solve2();

class StringHash {
    // h[i] 是前 i 个字符 s[0..i) 的哈希；pw[i] 是 base 的 i 次幂。
    vector<size_t> h, pw;
    size_t base = 65535;

public:
    explicit StringHash(const string &s) : h(s.length() + 1), pw(s.length() + 1, 1) {
        for (int i = 0; i < s.length(); i++) {
            // 加入新字符：原哈希乘 base，再加上当前字符值。
            // size_t 为无符号整数，溢出会按其位宽自动取模。
            h[i + 1] = h[i] * base + static_cast<size_t>(s[i]);
            pw[i + 1] = pw[i] * base;
        }
    }

    // 返回半开区间 s[l..r) 的哈希值：
    // 大前缀减去左侧前缀向高位移动 r-l 位后的值。
    size_t operator()(int l, int r) const {
        return h[r] - h[l] * pw[r - l];
    }
};

// 前缀哈希版本：预处理 O(l)，随后每个候选周期只需进行一次哈希比较。
void solve2() {
    int l;
    string s;
    cin >> l >> s;
    StringHash sh(s);
    for (int p = 1; p <= l; p++) {
        // 两段长度均为 l-p；相等说明 p 满足周期条件。
        if (sh(0, l - p) == sh(p, l)) {
            cout << p << '\n';
            break;
        }
    }
}

int main() {
    // 使用满足本题数据规模的前缀哈希版本。
    solve2();
    return 0;
}
