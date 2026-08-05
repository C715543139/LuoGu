/**
 * Luogu P1638 - 逛画展
 *
 * 问题描述：
 *   展览按顺序摆放 n 幅画，第 i 幅画由画家 a[i] 创作。
 *   购买区间 [x, y] 的门票可观看其中所有画，票价为 y-x+1。
 *   求包含全部 m 位画家作品的最短区间；若长度相同，输出左端点更小的一组。
 *
 * 解法：双指针（滑动窗口）
 *   1. 用 include 记录当前窗口内每位画家的作品数量，
 *      用 exclude 记录当前窗口尚未包含的画家。
 *   2. 窗口缺少画家时，右指针 q 右移并将新画加入窗口。
 *   3. 窗口已包含全部画家时，记录当前区间，再右移左指针 p 缩小窗口。
 *   4. 每幅画最多被加入和移出窗口各一次，因此扫描过程为线性时间。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n)，哈希表和哈希集合操作的期望时间均为 O(1)
 *   - 空间复杂度：O(m)，用于保存各画家的窗口计数和缺失状态
 */

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    // 读入按展览顺序排列的画家编号
    vector v(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    // include[painter]：当前窗口中 painter 的作品数量
    unordered_map<int, int> include;
    // exclude：当前窗口尚未包含的画家；初始时所有画家均缺失
    unordered_set<int> exclude;
    for (int i = 1; i <= m; i++) {
        exclude.insert(i);
    }

    // p、q：当前窗口 [p, q] 的左右端点；q=-1 表示初始空窗口
    // x、y：当前最优区间的左右端点（均为 0-based）
    int p = 0, q = -1, x = 0, y = n - 1;
    // 仍可扩展右端点，或窗口完整且可继续收缩左端点时，继续处理
    while (q < n - 1 || exclude.empty()) {
        if (!exclude.empty()) {
            // 缺少画家：右移 q，将下一幅画加入窗口
            q++;
            include[v[q]]++;
            // 该画家已在窗口中出现，不再属于缺失集合
            exclude.erase(v[q]);
        } else {
            // 当前窗口包含全部 m 位画家，尝试用更短区间更新答案
            if (q - p < y - x) {
                x = p;
                y = q;
            }
            // 长度相同时不更新：较小左端点的等长区间会更早被扫描到

            // 移除左端画作，继续尝试缩小窗口
            if (include.count(v[p]) > 0) {
                include[v[p]]--;
                if (include[v[p]] == 0) {
                    // 移走该画家的最后一幅作品后，窗口重新缺少该画家
                    exclude.insert(v[p]);
                }
            }
            p++;
        }
    }

    // 下标从 0-based 转为题目要求的 1-based 后输出
    cout << x + 1 << " " << y + 1 << endl;
    return 0;
}
