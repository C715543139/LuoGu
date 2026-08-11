/**
 * Luogu P3375 - 【模板】KMP
 *
 * 问题描述：
 *   求模式串 s2 在文本串 s1 中的全部出现位置，
 *   并输出 s2 每个前缀的最长 border 长度。
 *
 * 解法：KMP
 *   pi[i] 表示 s2[0..i] 的最长 border 长度，即同时是该前缀开头和结尾的最长真子串长度。
 *   匹配过程中，j 表示当前已匹配的 s2 前缀长度。
 *   失配时令 j 回退到 pi[j - 1]，保留已匹配部分中仍可作为模式串前缀的后缀；
 *   文本串指针无需后退。完整匹配后同样回退，以识别可能重叠的出现位置。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(|s1| + |s2|)
 *   - 空间复杂度：O(|s2|)
 *
 * 代码说明：
 *   solve1 保留直接查找与枚举 border 的对照实现；main 实际调用 solve2 的 KMP 实现。
 */

#include <iostream>
#include <vector>
using namespace std;

// 直接做法的入口，保留用于和 KMP 做对照。
void solve1();

// 检查长度为 l 的候选串，是否同时等于 s 的前缀、从 idx 开始的子串和
// 长度为 sL 的前缀的后缀。solve1 用它判断某个前缀的 border。
bool check(string &s, int sL, int idx, int l) {
    for (int i = 0; i < l; i++) {
        if (s[i] != s[idx + i] || s[idx + i] != s[sL - l + i]) {
            return false;
        }
    }
    return true;
}

// 直接查找版本：逻辑直观，但重复比较较多，不适用于本题的最大数据规模。
void solve1() {
    string s1, s2;
    cin >> s1 >> s2;
    int l2 = s2.length();

    // 从当前位置起查找完整子串；循环变量每轮加一，因此能够找出重叠出现的位置。
    for (unsigned long long i = 0; i != string::npos; i++) {
        i = s1.find(s2, i);
        if (i != string::npos) {
            cout << i + 1 << '\n';
        } else {
            break;
        }
    }

    // 长度为 1 的前缀没有非自身的 border。
    cout << 0 << ' ';
    for (int i = 2; i <= l2; i++) {
        // 枚举当前前缀长度 i 的所有候选 border 长度。
        bool flag = false;
        for (int j = i - 1; j > 0; j--) {
            // 从长到短尝试，首次成功的 j 就是最长 border 长度。
            if (check(s2, i, 0, j)) {
                cout << j << ' ';
                flag = true;
                break;
            }
        }
        if (!flag) {
            cout << 0 << ' ';
        }
    }
}

// KMP 版本：main 实际调用的线性时间实现。
void solve2() {
    string s1, s2;
    cin >> s1 >> s2;

    int l1 = s1.length(), l2 = s2.length();

    // pi[i]：模式串 s2[0..i] 的最长 border 长度。
    vector pi(l2, 0);
    for (int i = 1, j = 0; i < l2;i++) {
        // j 是当前尝试保留的 border 长度；失配时沿更短的 border 链回退。
        while (j > 0 && s2[j] != s2[i]) {
            j = pi[j - 1];
        }

        // 当前字符可接在已有 border 后面，border 长度增加一。
        if (s2[j] == s2[i]) {
            j++;
        }
        pi[i] = j;
    }

    // 扫描文本串。j 表示文本当前位置之前，已连续匹配的模式串前缀长度。
    for (int i = 0, j = 0; i < l1;i++) {
        // 当前字符失配时，文本下标 i 保持不动，只缩短可复用的模式串前缀长度。
        while (j > 0 && s1[i] != s2[j]) {
            j = pi[j - 1];
        }

        // 当前文本字符与模式串的下一个待匹配字符相同。
        if (s1[i] == s2[j]) {
            j++;
        }

        if (j == l2) {
            // i 是匹配终点的 0-based 下标，题目要求输出起点的 1-based 下标。
            cout << i - l2 + 2 << '\n';

            // 保留最长可复用后缀，避免漏掉重叠出现的位置。
            j = pi[j - 1];
        }
    }

    // pi 数组正是题目要求输出的每个模式串前缀的最长 border 长度。
    for (int i = 0; i < l2; ++i) {
        cout << pi[i] << ' ';
    }
}

int main() {
    // 使用满足题目数据范围的 KMP 实现。
    solve2();
    return 0;
}
