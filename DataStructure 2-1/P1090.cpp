/**
 * Luogu P1090 - [NOIP 2004 提高组] 合并果子
 *
 * 问题描述：
 *   给定 n 堆果子的重量。每次选择两堆合并，消耗的体力等于两堆重量之和；
 *   新堆还会参与后续合并。求最终合成一堆所需的最小总体力。
 *
 * 解法：贪心 + 小根堆（Huffman 合并）
 *   每次选择当前最轻的两堆进行合并，将本次合并重量计入答案，再把新堆放回。
 *   一堆果子合并得越早，它的重量就可能在后续合并中被重复计算越多次，因此应让
 *   较轻的果子承担更多次计算。任意最优合并方案都可以调整为让当前最轻的两堆先
 *   合并，合并后的问题仍是相同形式，重复这一选择即可得到全局最优解。
 *
 *   使用 greater<> 配置 priority_queue，使堆顶始终是当前最小重量，从而能高效
 *   取出两堆最轻果子并插入合并后的新堆。
 *
 * 复杂度分析：
 *   共进行 n-1 次合并，每次包含常数次堆操作：
 *   - 时间复杂度：O(n log n)
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector v(n, 0);
    for (int i = 0; i < n; i++) cin >> v[i];

    // count 累加每一次合并所消耗的体力。
    int count = 0;
    // greater<> 将 priority_queue 配置为小根堆，使堆顶为当前最轻的一堆。
    priority_queue<int, vector<int>, greater<>> pq;
    // 初始时将所有果子堆放入小根堆。
    for (int i: v) pq.push(i);
    // 每次合并会让堆的数量减少一，最终只剩一堆时结束。
    while (pq.size() > 1) {
        // 取出当前最轻的一堆。
        int top = pq.top();
        pq.pop();
        // 此时新的堆顶是第二轻的一堆；top 随后保存两堆的合并重量。
        top += pq.top();
        // 本次体力消耗等于新堆重量，将其计入总答案。
        count += top;
        pq.pop();
        // 新堆还需要参与后续合并，因此重新放回小根堆。
        pq.push(top);
    }
    cout << count << endl;
    return 0;
}
