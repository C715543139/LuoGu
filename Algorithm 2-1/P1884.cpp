/**
 * Luogu P1884 - [USACO12FEB] Overplanting S
 *
 * 问题描述：
 *   给定 n 个矩形（用左下角 (x1, y1) 与右上角 (x2, y2) 表示），
 *   求这 n 个矩形覆盖的总面积（重叠部分只算一次）。
 *
 * 解法：扫描线 + 离散化
 *   1. 将所有矩形按 x1 排序，模拟从左到右的扫描过程
 *   2. 对当前"切片"（所有 x1 相同的矩形）：
 *      - 在 y 方向做一次区间合并，求出当前切片在 y 上的覆盖长度
 *      - 切片宽度 = 下一段不同的 x1 与当前 x1 的差（即 nextX - x）
 *      - 当前切片贡献的面积 = 合并后 y 长度 × 切片宽度
 *   3. 将切片中未结束（x2 > nextX）的矩形"截断"后放回，
 *      使其 x1 推进到 nextX，继续后续扫描
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n^2)（每次切片处理可能涉及区间合并与重排序）
 *   - 空间复杂度：O(n)
 *   - 适用于矩形数量较小的情况；大规模可改用线段树扫描线 O(n log n)
 */

#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>
#include <climits>
using namespace std;


int main() {
    int n;
    cin >> n;  // 矩形数量
    // 用 deque 存储矩形，方便对前端切片处理后把未结束的矩形重新 push 回前端
    deque rectangles(n, vector<int>(4));

    // 读入每个矩形的左下角 (x1, y1) 与右上角 (x2, y2)
    for (int i = 0; i < n; i++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        rectangles[i][0] = x1;
        rectangles[i][1] = y1;
        rectangles[i][2] = x2;
        rectangles[i][3] = y2;
    }
    // 按 x1 升序排序，便于顺序扫描
    sort(rectangles.begin(), rectangles.end());

    long long sum = 0;  // 累计覆盖面积
    // 持续处理直到所有矩形均结束
    while (!rectangles.empty()) {
        // sameStart: 本次切片中 x1 相同的所有矩形
        vector<vector<int>> sameStart;
        // intervals: 对应矩形在 y 方向的覆盖区间 [y1, y2]（排序时存为 (y2, y1)）
        vector<pair<int, int>> intervals;
        int x = rectangles.front()[0];  // 当前切片的起始 x 坐标
        // 取出所有 x1 == x 的矩形
        while (!rectangles.empty() && rectangles.front()[0] == x) {
            sameStart.push_back(rectangles.front());
            // intervals 存为 (y2, y1) 方便后续按右端点排序
            intervals.emplace_back(rectangles.front()[3], rectangles.front()[1]);
            rectangles.pop_front();
        }

        // 计算切片的右端点 nextX：
        // 即当前切片中所有矩形 x2 的最小值（若队列已空则视为无穷大）
        int nextX = !rectangles.empty() ? rectangles.front()[0] : INT_MAX;
        for (auto &rectangle : sameStart) {
            nextX = min(rectangle[2], nextX);
        }

        // 在 y 方向做区间合并，求当前切片在 y 上的覆盖长度
        sort(intervals.begin(), intervals.end());
        int lastHead = intervals.front().first, lastTail = intervals.front().second;
        for (int i = 1; i < intervals.size(); i++) {
            int head = intervals[i].first, tail = intervals[i].second;
            if (head > lastTail) {
                // 当前区间与已合并区间不相交
                // 累加这段 y 长度 × 切片宽度
                sum += (long long)(lastTail - lastHead) * (nextX - x);
                lastHead = head;
                lastTail = tail;
            } else {
                // 相交则合并：取较大右端点
                lastTail = max(tail, lastTail);
            }
        }
        // 累加最后一段 y 区间对应的面积
        sum += (long long)(lastTail - lastHead) * (nextX - x);

        // 把未结束（x2 > nextX）的矩形截断后放回 deque 前端
        // 其新的 x1 = nextX，继续参与下一轮扫描
        for (auto &rectangle : sameStart) {
            if (rectangle[2] > nextX) {
                rectangle[0] = nextX;
                rectangles.push_front(rectangle);
            }
        }
    }
    cout << sum << endl;
    return 0;
}
