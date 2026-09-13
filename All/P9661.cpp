/**
 * Luogu P9661 - [ICPC 2021 Macao R] Sandpile on Clique
 *
 * 问题描述：
 *   给定一个包含 n 个节点的完全图，第 i 个节点初始有 a[i] 枚筹码。当某个节点
 *   拥有至少 n-1 枚筹码时，可以令它 firing：该节点向其余每个节点各发送一枚
 *   筹码，因此自身减少 n-1 枚筹码。判断这一过程是否会无限持续；若会则输出
 *   Recurrent，否则输出过程停止后每个节点的筹码数。
 *
 * 解法：排序 + 差分数组 + 批量 firing
 *   vals 按筹码数从大到小保存 {筹码数, 原下标}，subVals 是原下标顺序下筹码数
 *   的差分数组。处理某个节点时，通过 times 一次合并它能够连续进行的 firing：
 *   - 自身共减少 times*(n-1)；
 *   - 其余每个节点共增加 times。
 *   上述修改都转化为 subVals 上的区间或单点差分修改，随后通过前缀和恢复各节点
 *   的当前筹码数并重新排序。sum 记录本轮此前节点 firing 给后续节点带来的增量。
 *   根据完全图沙堆的判定性质，若被批量处理的节点次数达到 n+1，过程将循环；若
 *   当前最大筹码数小于 n-1，则所有节点都无法继续 firing，输出最终状态。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n^2 log n)
 *   - 空间复杂度：O(n)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    // vals[i] 保存 {筹码数, 原下标}；subVals 按原下标保存筹码数的差分
    vector vals(n, pair<long long, int>(0, 0));
    vector<long long> subVals(n, 0);
    for (int i = 0; i < n; ++i) {
        cin >> vals[i].first;
        vals[i].second = i;
        // 将输入序列原地转换为差分形式，便于后续进行批量区间修改
        subVals[i] = vals[i].first;
        if (i > 0) {
            subVals[i] -= vals[i - 1].first;
        }
    }

    // 优先处理当前筹码数较多的节点
    sort(vals.begin(), vals.end(), [](const pair<long long, int> &a, const pair<long long, int> &b) {
        return a.first > b.first;
    });

    // count 记录已经执行过的批量 firing 处理次数，用于判断是否进入循环状态
    int count = 0;
    while (vals[0].first >= n - 1) {
        // 本轮此前节点 firing 后，会给尚未处理的每个节点增加 sum 枚筹码
        long long sum = 0;
        for (int i = 0; i < n; ++i) {
            // 后续节点的筹码数不超过当前节点，当前节点不能 firing 时可结束本轮
            if (vals[i].first + sum < n - 1) {
                break;
            }

            auto [val, idx] = vals[i];
            // 将该节点当前能够进行的多次 firing 合并处理
            auto times = (val + sum) / (n - 1);
            sum += times;

            // 该节点每次 firing 都会损失 n-1 枚筹码：对原位置 idx 做单点修改
            subVals[idx] -= times * (n - 1);
            if (idx != n - 1) {
                subVals[idx + 1] += times * (n - 1);
            }

            // 先给所有节点增加 times，再抵消节点 idx 自身不应获得的 times
            subVals[0] += times;
            subVals[idx] -= times;
            if (idx != n - 1) {
                subVals[idx + 1] += times;
            }

            count++;
            // 完全图中达到该处理次数说明 firing 不会终止
            if (count >= n + 1) {
                cout << "Recurrent";
                return 0;
            }
        }

        // 对差分数组求前缀和，恢复各节点的当前筹码数
        for (int i = 0; i < n; ++i) {
            vals[i].first = subVals[i];
            vals[i].second = i;
            if (i > 0) {
                vals[i].first += vals[i - 1].first;
            }
        }

        // 为下一轮重新按照筹码数降序排列
        sort(vals.begin(), vals.end(), [](const pair<int, int> &a, const pair<int, int> &b) {
            return a.first > b.first;
        });
    }

    // 最大值已经小于 n-1，恢复并按原节点顺序输出稳定状态
    for (int i = 0; i < n; ++i) {
        vals[i].first = subVals[i];
        if (i > 0) {
            vals[i].first += vals[i - 1].first;
        }
        cout << vals[i].first;
        if (i != n - 1) {
            cout << " ";
        }
    }
    return 0;
}
