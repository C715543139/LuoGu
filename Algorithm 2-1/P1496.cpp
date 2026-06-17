/**
 * Luogu P1496 - 火烧赤壁
 *
 * 问题描述：
 *   在数轴上有 n 条线段，每条线段用 (first, second) 表示其左右端点。
 *   求这 n 条线段覆盖的总长度（即合并所有相交/重叠线段后的并集长度）。
 *
 * 解法：排序 + 区间合并
 *   1. 将所有线段按左端点升序排序
 *   2. 维护当前合并区间的左端点 head 与右端点 tail
 *   3. 依次处理每条线段：
 *      - 若当前线段左端点 > tail，说明与前一个合并区间不相交，
 *        累加 tail - head 到答案，并开启新的合并区间；
 *      - 否则，更新 tail = max(tail, 当前线段右端点)，进行区间合并
 *   4. 循环结束后再累加最后一段区间长度
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n log n)，主要来自排序
 *   - 空间复杂度：O(n)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;  // 线段数量
    vector<pair<long long, long long>> v(n);
    // 读入每条线段的左右端点
    for (int i = 0; i < n; i++) cin >> v[i].first >> v[i].second;
    // 按左端点升序排序，便于后续顺序合并
    sort(v.begin(), v.end());

    // head: 当前合并区间的左端点
    // tail: 当前合并区间的右端点
    // sum : 已合并区间的累计长度
    long long head = v[0].first, tail = v[0].second;
    long long sum = 0;
    // 从第二条线段开始，依次与当前合并区间比较
    for (int i = 1; i < n; i++) {
        long long curHead = v[i].first, curTail = v[i].second;
        if (curHead > tail) {
            // 当前线段与合并区间不相交（甚至不接触）
            // 累加当前合并区间的长度
            sum += tail - head;
            // 开启新的合并区间
            head = curHead;
            tail = curTail;
        } else {
            // 当前线段与合并区间相交或相接
            // 合并到当前区间中（右端点取较大值）
            tail = max(tail, curTail);
        }
    }
    // 累加最后一段合并区间的长度（循环结束后 tail 仍指向最后一段）
    sum += tail - head;
    cout << sum << endl;
    return 0;
}
