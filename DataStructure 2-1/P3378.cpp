/**
 * Luogu P3378 - 【模板】堆
 *
 * 问题描述：
 *   维护一个初始为空的数列，支持插入整数、查询最小值和删除一个最小值。
 *
 * 解法：小根堆
 *   使用 priority_queue 保存数列中的所有元素，并将比较器设为 greater<>，使堆顶
 *   始终是当前最小值。插入操作调用 push；查询操作读取 top；删除操作调用 pop。
 *   堆只保证堆顶元素最小，无需维护其余元素的完整有序关系，因此插入和删除只需
 *   沿堆的一条路径调整。
 *
 * 复杂度分析：
 *   设当前堆中有 m 个元素：
 *   - 插入：O(log m)
 *   - 查询最小值：O(1)
 *   - 删除最小值：O(log m)
 *   - 处理全部 n 次操作：O(n log n)
 *   - 空间复杂度：O(n)
 *
 * 代码说明：
 *   操作 2 和操作 3 直接访问堆顶，依赖输入保证执行这些操作时堆非空。
 */

#include <iostream>
#include <queue>
using namespace std;

int main() {
    // 最多读入 10^6 次操作，关闭同步并解除绑定以加快输入输出。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    // priority_queue 默认是大根堆；greater<> 使较小元素具有更高优先级，构成小根堆。
    priority_queue<int, vector<int>, greater<>> pq;
    for (int i = 0; i < n; i++) {
        int op;
        cin >> op;
        if (op == 1) {
            // 操作 1：将 x 插入堆中，并自动调整以恢复小根堆性质。
            int x;
            cin >> x;
            pq.push(x);
        } else if (op == 2) {
            // 操作 2：小根堆的堆顶就是当前数列中的最小值。
            cout << pq.top() << '\n';
        } else {
            // 操作 3：删除一个堆顶元素；若最小值重复，也只会删除其中一个。
            pq.pop();
        }
    }
    return 0;
}
