/**
 * Luogu P2161 - [SHOI2009] 会场预约
 *
 * 问题描述：
 *   维护若干个互不相交的闭区间。操作 A l r 会删除所有与新区间 [l,r] 相交的
 *   旧区间，再加入新区间，并输出删除数量；操作 B 输出当前区间数量。
 *
 * 解法：有序集合 + 以新区间为锚点向两侧检查
 *   使用 set<pair<int,int>> 保存所有预约，pair 的 first、second 分别表示区间的
 *   左右端点。set 按左端点优先排序，因此新区间插入后：
 *   1. 它左侧的旧区间中，最多只有紧邻的一个区间可能与它相交；
 *   2. 它右侧所有左端点不大于 r 的旧区间都与它相交，可以依次删除；
 *   3. 遇到第一个左端点大于 r 的区间后，后续区间也不会与新区间相交。
 *
 *   原集合中的区间始终互不相交，这是“左侧最多检查一个区间”的依据。如果插入
 *   失败，说明集合中已有完全相同的区间。按照题意，旧区间应被删除一次后再加入
 *   同一区间；集合状态不变，删除数量记为 1 即可。
 *
 * 复杂度分析：
 *   设一次 A 操作删除 k 个区间：
 *   - 插入新区间：O(log |S|)
 *   - 删除相交区间：O(k)，set 按迭代器删除的复杂度为均摊 O(1)
 *   - 查询区间数量：O(1)
 *   每个区间只会被插入一次、删除一次，全部 n 次操作的总时间复杂度为 O(n log n)，
 *   空间复杂度为 O(n)。
 *
 * 代码说明：
 *   预约区间是闭区间，端点相同也属于相交。因此左侧判断使用 previous->second >= l，
 *   右侧判断使用 right->first <= r。
 */

#include <iostream>
#include <set>
#include <utility>
using namespace std;

int main() {
    // 操作数量最多为 2*10^5，关闭同步并解除绑定以加快输入输出。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // reservations 按照“左端点优先、右端点次之”的顺序保存当前有效预约。
    set<pair<int, int>> reservations;
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        char c;
        cin >> c;
        if (c == 'B') {
            // 操作 B：set 的元素数量就是当前有效预约数量。
            cout << reservations.size() << '\n';
        } else {
            int l, r;
            cin >> l >> r;

            // 先插入新区间，current 作为检查左右相邻区间的固定锚点。
            auto result = reservations.insert({l, r});
            auto current = result.first;
            int count = 0;

            if (!result.second) {
                // 已存在完全相同的预约：逻辑上删除旧预约再插入新区间，集合状态不变。
                count = 1;
            } else {
                // 由于原有预约互不相交，左侧最多只有一个预约可能与新区间相交。
                if (current != reservations.begin()) {
                    auto previous = current;
                    --previous;
                    if (previous->second >= l) {
                        // 左侧预约的右端点到达 l，两个闭区间相交。
                        reservations.erase(previous);
                        count++;
                    }
                }

                // 右侧所有起点不大于 r 的预约都与新区间相交。
                auto right = current;
                ++right;
                while (right != reservations.end() && right->first <= r) {
                    // erase 返回被删除元素的下一个迭代器，可以继续安全遍历。
                    right = reservations.erase(right);
                    count++;
                }
            }

            // 操作 A：输出为了接受新区间而删除的旧预约数量。
            cout << count << '\n';
        }
    }
    return 0;
}
