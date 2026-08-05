/**
 * Luogu P2882 - [USACO07MAR] Face The Right Way G（反转奶牛）
 *
 * 问题描述：
 *   有 N 头奶牛排成一列，初始朝向为 F（朝前）或 B（朝后）。
 *   每次操作选择连续 K 头奶牛，并将它们的朝向全部翻转。
 *   需要选择一个 K，使所有奶牛最终朝前，且翻转次数 M 最少；输出 K 和 M。
 *
 * 解法：异或差分 + 枚举翻转长度 + 贪心
 *   1. 用 v[i] 表示第 i 头奶牛的朝向：B 为 1，F 为 0。
 *   2. 构造异或差分 d：d[i] = v[i] XOR v[i-1]，其中 v[0] = 0。
 *      翻转区间 [l, l+K-1] 时，只有 d[l] 与 d[l+K] 会被翻转，
 *      因为区间内部相邻奶牛会同时翻转，二者的异或关系保持不变。
 *   3. 枚举每个可能的翻转长度 K。对固定的 K 从左到右扫描 d：
 *      - 若 d[i] 为 1，前面已经处理过的位置无法再影响它，必须从 i 开始翻转；
 *      - 若 d[i] 为 0，无需翻转；
 *      - 扫描后若尾部仍有 1，说明剩余奶牛无法凑满 K 头，当前 K 不可行。
 *   4. 在所有可行的 K 中保留翻转次数最少的一组答案。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(N^2)，枚举 K 为 O(N)，每个 K 扫描一次差分数组为 O(N)
 *   - 空间复杂度：O(N)
 */

#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N;
    cin >> N;

    // v[i] = true 表示面朝后（B），false 表示面朝前（F）；下标从 1 开始
    vector v(N + 1, false);
    // 读入每头奶牛的初始朝向
    for (int i = 1; i <= N; i++) {
        char c;
        cin >> c;
        v[i] = c == 'B';
    }

    // 构造异或差分：d[i] = v[i] XOR v[i-1]，其中 v[0] 由初始化保证为 0
    vector d(N + 1, 0);
    for (int i = 1; i <= N; i++) {
        if (v[i] != v[i - 1]) {
            d[i] = 1;
        }
    }

    // K：当前最优翻转长度；M：对应的最少翻转次数
    int K = N + 1, M = N + 1;
    // 枚举固定的翻转长度 j
    for (int j = 1; j <= N; j++) {
        int count = 0;
        // 当前 j 下的差分状态，避免影响下一轮枚举
        vector dCopy = d;
        // 只扫描能作为长度为 j 的翻转区间左端点的位置
        for (int i = 1; i + j <= N + 1; i++) {
            if (dCopy[i] == 1) {
                // dCopy[i] 已无法被此前的翻转改变，必须从 i 开始翻转
                dCopy[i] = 0;
                // 翻转区间 [i, i+j-1] 会翻转右端点差分 d[i+j]
                if (i + j <= N) {
                    dCopy[i + j] ^= 1;
                }
                // i+j=N+1 时右端点落在末尾哨兵，忽略即可
                count++;
            }
        }

        // 检查无法再作为翻转起点的尾部差分是否已全部消除
        bool flag = true;
        for (int i = N - j + 2; i <= N; i++) {
            if (dCopy[i] == 1) {
                flag = false;
                break;
            }
        }

        // 仅在翻转次数更少时更新；次数相同时保留更小的 j
        if (flag && M > count) {
            M = count;
            K = j;
        }
    }

    // 输出最优翻转长度及对应的最少翻转次数
    cout << K << " " << M << endl;
    return 0;
}
