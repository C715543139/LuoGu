/**
 * Luogu P1168 - 中位数
 *
 * 问题描述：
 *   给定一个长度为 n 的序列。每读入奇数个元素，就输出当前前缀的中位数。
 *   对长度为 2t+1 的前缀，中位数是升序排列后的第 t+1 个元素。
 *
 * 代码中保留了两种做法：
 *
 * 解法一：PBDS 顺序统计树（solve1）
 *   将每个元素存为 (数值, 下标)，用不同下标区分数值相同的元素。
 *   find_by_order(k) 可以在 O(log n) 时间内取得 0-based 排名为 k 的元素，
 *   因此读入下标 i 为偶数时，0-based 排名为 i/2 的元素就是当前中位数。
 *
 * 解法二：对顶堆（solve2，main 当前调用）
 *   使用大根堆 aPQ 保存较小的一半元素，小根堆 iPQ 保存较大的一半元素，
 *   并始终保持 aPQ 中的有效元素不大于 iPQ 中的有效元素。
 *
 *   两个堆分别加入 INT_MIN 和 INT_MAX 作为哨兵，使取堆顶时无需判断空堆。
 *   需要输出中位数时调整两个堆：计入哨兵后，让 iPQ 比 aPQ 多一个元素。
 *   对于 2t+1 个有效元素，此时 aPQ 含有 INT_MIN 和较小的 t 个元素，
 *   iPQ 含有较大的 t+1 个元素和 INT_MAX，所以 iPQ.top() 正是中位数。
 *
 * 复杂度分析：
 *   - 两种做法的时间复杂度均为 O(n log n)
 *   - 空间复杂度均为 O(n)
 */

#include <iostream>
#include <queue>
#include <climits>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;

// 解法一：使用 PBDS 顺序统计树直接查询当前第 k 小元素。
void solve1() {
    int n;
    cin >> n;

    // PBDS tree 不保存重复键，因此用下标 i 区分数值相同的元素。
    // pair 先按数值、再按下标排序，其顺序与原数值的升序一致。
    __gnu_pbds::tree<pair<int, int>, __gnu_pbds::null_type, less<>,
        __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> os;
    for (int i = 0; i < n; i++) {
        int num;
        cin >> num;
        os.insert({num, i});

        // i 为偶数时，当前前缀长度 i+1 为奇数；中位数的 0-based 排名为 i/2。
        if (i % 2 == 0) {
            cout << os.find_by_order(i / 2)->first << '\n';
        }
    }
}

// 解法二：使用大根堆与小根堆共同维护中位数。
void solve2() {
    int n;
    cin >> n;

    // aPQ：大根堆，保存较小的一半；iPQ：小根堆，保存较大的一半。
    priority_queue<int> aPQ;
    priority_queue<int, vector<int>, greater<>> iPQ;
    // 哨兵保证两个堆始终非空，不参与实际序列的中位数计算。
    aPQ.push(INT_MIN);
    iPQ.push(INT_MAX);
    for (int i = 0; i < n; i++) {
        int num;
        cin >> num;

        // 以较大一半的最小值为分界，将新元素放入对应的堆。
        if (num < iPQ.top()) {
            aPQ.push(num);
        } else {
            iPQ.push(num);
        }

        // 只在前缀长度为奇数、需要输出答案时调整两个堆的大小。
        if (i % 2 == 0) {
            // 移动堆顶不会破坏“aPQ 中元素 <= iPQ 中元素”的分界关系。
            while (iPQ.size() > aPQ.size()) {
                aPQ.push(iPQ.top());
                iPQ.pop();
            }
            while (aPQ.size() > iPQ.size()) {
                iPQ.push(aPQ.top());
                aPQ.pop();
            }
            // 调整后 iPQ 比 aPQ 多一个元素，iPQ.top() 即当前中位数。
            cout << iPQ.top() << '\n';
        }
    }
}

int main() {
    // 当前使用对顶堆解法；solve1 保留为 PBDS 的另一种实现。
    solve2();
    return 0;
}
