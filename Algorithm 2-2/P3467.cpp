/**
 * Luogu P3467 - [POI 2008] PLA-Postering
 *
 * 问题描述：
 *   一排相邻建筑的高度依次给出。用若干不重叠的矩形海报完全覆盖北立面，
 *   求最少需要的海报数量。
 *
 * 本文件包含两个版本：
 *   - solve1：按最低高度分割连续区间的模拟写法，用于理解，最坏为 O(n^2)。
 *   - solve2：单调递增栈写法，当前 main 调用此版本，时间复杂度为 O(n)。
 *
 * solve2 的核心：
 *   1. 栈保存仍可向右延伸的高度边界，栈内高度严格递增。
 *   2. 当前建筑更低时，高于它的海报无法继续延伸，依次出栈。
 *   3. 当前高度高于栈顶时，出现一段此前不存在的新墙面，必须新开一张海报。
 *   4. 当前高度等于栈顶时，已有海报可继续覆盖，无需新增。
 *
 * 复杂度分析（solve2）：
 *   - 时间复杂度：O(n)，每个高度最多入栈、出栈各一次
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <list>
#include <stack>
#include <vector>
using namespace std;

/**
 * 解法一：按当前最左连续区间的最低高度模拟覆盖。
 *
 * 每次删除该区间内所有最低位置，相当于覆盖这一层最低的横向海报。
 * 删除后，剩余位置会被分为若干连续区间，再逐段处理。
 *
 * 该方法逻辑直观，但严格递增高度时会反复扫描长区间，最坏 O(n^2)，
 * 因而仅保留作思路对照。
 */
void solve1() {
    int n;
    cin >> n;
    // 读入建筑高度；宽度只影响实际横向长度，不影响最少海报张数
    vector v(n, 0);
    for (int i = 0; i < n; i++) {
        int w;
        cin >> w >> v[i];
    }

    // next 保存尚未被当前模拟层覆盖的位置下标
    int count = 0;
    list<int> next;
    for (int i = 0; i < n; i++) next.push_back(i);
    while (!next.empty()) {
        // 在最左侧连续区间中寻找最低高度，并记录所有最低位置
        int base = v[next.front()], last = next.front();
        auto done = vector{next.begin()};

        for (auto it = ++next.begin(); it != next.end(); ++it) {
            if (*it != last + 1) {
                break;
            }
            last = *it;
            if (v[*it] < base) {
                base = v[*it];
                done.clear();
                done.push_back(it);
            } else if (v[*it] == base) {
                done.push_back(it);
            }
        }

        // 用一张海报覆盖该最低层，删除这些位置后形成新的连续区间
        for (auto &it: done) {
            next.erase(it);
        }
        count++;
    }
    cout << count << endl;
}

/**
 * 解法二：单调递增栈。
 *
 * 栈中的高度均对应尚未结束、还能向右延伸的海报上边界。
 * 建筑高度为正，因此空栈可视为高度为 0 的地面边界。
 */
void solve2() {
    int n;
    cin >> n;
    // 读入建筑高度；宽度无需参与计算
    vector v(n, 0);
    for (int i = 0; i < n; i++) {
        int w;
        cin >> w >> v[i];
    }

    // count：已开启的海报数量；st：高度严格递增的活动边界
    int count = 0;
    stack<int> st;
    // 从左到右处理每栋建筑
    for (int i = 0; i < n; i++) {
        // 高于当前建筑的海报无法延伸到当前位置，在此结束
        while (!st.empty() && st.top() > v[i]) st.pop();
        // 当前高度高于活动边界时，新增一段墙面，必须开启一张海报
        if (st.empty() || st.top() < v[i]) {
            st.push(v[i]);
            count++;
        }
        // 若 st.top() == v[i]，已有海报可直接延伸，无需新增
    }
    // 输出最少海报数量
    cout << count << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 使用线性复杂度的单调栈版本
    solve2();
    return 0;
}
