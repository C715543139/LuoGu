/**
 * Luogu P1904 - 天际线
 *
 * 问题描述：
 *   给出 n 栋建筑，每栋用 (l, h, r) 描述其左端点 l、高度 h、右端点 r。
 *   城市的天际线是这些建筑叠加后形成的多边形上沿。
 *   要求按 x 坐标升序输出天际线上所有"关键点"：
 *     (x1, h1, x2, h2, x3, h3, ...)
 *   即输出高度发生变化的 x 坐标与对应高度，以及最后下降到 0 的点。
 *
 * 解法：分块扫描（按 l 分组）
 *   1. 将所有建筑按左端点 l 升序排序
 *   2. 维护当前天际线高度 curH
 *   3. 每次循环处理一批 l 相同的建筑：
 *      - 取出这批建筑 dealing，并计算
 *          curL: 当前批次的左端点（即 x 起点）
 *          curR: 当前批次中所有建筑 r 的最大值
 *          nextL: 当前批次中所有建筑 r 的最小值
 *      - downFlag: 当 curR < 下一批次的 l 时，说明本批 r 之后高度立刻降到 0
 *      - 对本批中所有建筑：
 *          * 记录其最大高度 maxH
 *          * 若该建筑 r > nextL，说明它在 nextL 之后还延伸出去，
 *            把它的 l 改为 nextL 后重新放回 deque 末端，参与后续扫描
 *      - 若 curH 与 maxH 不等：输出新高度关键点 (curL, maxH)
 *      - 若发生下界跌落或已到末尾：输出下降点 (curR, 0)
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n log n)，主要来自排序
 *   - 空间复杂度：O(n)
 *   - 该实现采用"分块+截断"思路，规避了完整的扫描线 O(n log n) 数据结构开销
 */

#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // buildings: 按 l 升序的建筑序列，用 deque 便于从前端弹出 l 相同的批次
    deque<vector<int>> buildings;
    int l, h, r, n = 0;
    // 不断读入 (l, h, r)，EOF 结束
    while (cin >> l >> h >> r) {
        buildings.push_back({l, h, r});
        n++;
    }
    // 按左端点 l 升序排序（默认按 vector 的字典序：(l, h, r)）
    sort(buildings.begin(), buildings.end());

    // curH: 当前天际线高度
    int curH = 0;
    // 持续处理直到所有建筑片段被消化
    while (!buildings.empty()) {
        // dealing: 当前批次（l 相同）的所有建筑
        vector<vector<int>> dealing;
        // nextL: 本批建筑中 r 的最小值（即本批次最先结束的位置）
        // curL : 本批次的左端点 x
        // curR : 本批次中 r 的最大值（本批次最远延伸的位置）
        int nextL = 65535, curL = buildings.front()[0], curR = buildings.front()[2];
        // 取出所有 l == curL 的建筑
        while (!buildings.empty() && buildings.front()[0] == curL) {
            dealing.push_back(buildings.front());
            // 更新本批次的 nextL（最小 r）和 curR（最大 r）
            nextL = min(buildings.front()[2], nextL);
            curR = max(buildings.front()[2], curR);
            buildings.pop_front();
        }

        // downFlag: 当前批次处理完后是否存在"立即下降"（中间无其他建筑承接）
        bool downFlag = false;
        if (!buildings.empty()) {
            // 若下一批次的 l 比本批次所有 r 都小，则本批次结束后会跌到 0
            nextL = min(buildings.front()[0], nextL);
            downFlag = curR < buildings.front()[0];
        }

        // 计算本批次中所有建筑的最大高度 maxH
        int maxH = 0;
        for (auto &building: dealing) {
            maxH = max(maxH, building[1]);
            // 若该建筑 r > nextL，说明它在 nextL 之后还继续延伸
            // 将其 l 推进到 nextL 后放回 deque 前端，参与后续扫描
            if (building[2] > nextL) {
                building[0] = nextL;
                buildings.push_front(building);
            }
        }

        // 若天际线高度发生变化，输出新的关键点
        if (curH != maxH) {
            curH = maxH;
            cout << curL << " " << curH << " ";
        }
        // 若发生下界跌落（中间无建筑），或已处理到末尾，输出下降点
        if ((downFlag && nextL != buildings.front()[0]) || buildings.empty()) {
            cout << curR << " " << 0 << " ";
        }
    }
    return 0;
}
