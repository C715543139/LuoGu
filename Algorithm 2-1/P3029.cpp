/**
 * Luogu P3029 - [USACO11NOV] Cow Lineup S
 *
 * 问题描述：
 *   每头奶牛有一个位置 x 和一个品种 ID。选择一段连续的位置范围拍照，
 *   照片成本为范围内最大 x 与最小 x 的差。
 *   求包含所有出现过的品种 ID 的最小照片成本。
 *
 * 解法：排序 + 双指针（滑动窗口）
 *   1. 按位置 x 升序排序。排序后任意照片范围都对应数组中的一个连续区间。
 *   2. 用 include 记录当前窗口内每个品种的出现次数，
 *      用 exclude 记录当前窗口尚未包含的品种。
 *   3. 若窗口缺少品种，右指针 q 右移以扩大窗口；
 *      若窗口已覆盖全部品种，先更新答案，再右移左指针 p 缩小窗口。
 *   4. 每头奶牛至多被 q 加入一次、被 p 移除一次，因此滑动窗口部分为线性时间。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(N log N)，主要来自排序；双指针扫描为 O(N)
 *   - 空间复杂度：O(N)，用于记录品种集合和窗口内计数
 */

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <climits>
using namespace std;

int main() {
    int N;
    cin >> N;

    // v[i] = (x, breed)：第 i 头奶牛的位置与品种；s 收集所有不同的品种 ID
    vector v(N, pair(0, 0));
    unordered_set<int> s;
    // 读入奶牛信息，并统计需要在照片中覆盖的全部品种
    for (int i = 0; i < N; i++) {
        int x, y;
        cin >> x >> y;
        v[i].first = x;
        v[i].second = y;
        s.insert(y);
    }

    // 按位置升序排序，使连续照片范围转化为数组上的连续子区间
    sort(v.begin(), v.end());

    // p、q：当前窗口 [p, q] 的左右端点；q=-1 表示初始空窗口
    // cost：目前找到的最小照片成本
    int p = 0, q = -1, cost = INT_MAX;
    // include[breed]：当前窗口内 breed 的出现次数
    unordered_map<int, int> include;
    // exclude：当前窗口中尚未出现的品种；为空时窗口覆盖全部品种
    unordered_set<int> exclude = s;

    // 仍可扩展右端点，或当前窗口完整且可继续收缩左端点时，继续处理
    while (q < N - 1 || exclude.empty()) {
        if (!exclude.empty()) {
            // 窗口缺少品种：右移 q，将一头奶牛加入窗口
            q++;
            include[v[q].second]++;
            // 该品种首次进入窗口后，不再属于缺失品种
            exclude.erase(v[q].second);
        } else {
            // 窗口已覆盖全部品种，记录当前范围的成本
            cost = min(cost, v[q].first - v[p].first);
            // 移除左端奶牛，尝试进一步缩小窗口
            if (include[v[p].second] > 0) {
                include[v[p].second]--;
                if (include[v[p].second] == 0) {
                    // 移走该品种的最后一头奶牛后，窗口重新缺少该品种
                    exclude.insert(v[p].second);
                }
            }
            p++;
        }
    }
    // 输出覆盖所有品种的最小位置跨度
    cout << cost << endl;
    return 0;
}
