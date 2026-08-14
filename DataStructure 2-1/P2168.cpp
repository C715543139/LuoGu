#include <iostream>
#include <queue>
using namespace std;

/**
 * Luogu P2168 - [NOI2015] 荷马史诗
 *
 * 问题描述：
 *   给 n 种单词分配 k 进制前缀编码，第 i 种单词出现次数为 w_i。
 *   首先要求所有单词的“出现次数 × 编码长度”之和最小；
 *   在总长度最小的方案中，还要让最长编码的长度尽可能短。
 *
 * 解法：k 叉 Huffman 树 + 小根堆
 *   前缀编码可以表示成一棵 k 叉树：每种单词对应一个叶子，
 *   叶子的深度就是该单词的编码长度。因此需要最小化树的带权路径长度。
 *
 *   每次从小根堆中取出权值最小的 k 棵子树，将它们合并成一棵新树：
 *   1. 新树权值等于 k 棵子树的权值之和；
 *   2. 合并后这些子树中每个叶子的深度都增加 1，
 *      所以答案增加这 k 棵子树的权值之和；
 *   3. 新树的最大深度等于各子树最大深度的最大值加 1。
 *
 *   当多个节点权值相同时，优先合并最大深度较小的节点，
 *   可以在带权路径长度相同的前提下，使最终树的最大深度最小。
 *
 *   完整 k 叉树的叶子数 N 满足 (N - 1) % (k - 1) == 0。
 *   若原叶子数不满足条件，就加入若干个权值为 0 的虚拟叶子；
 *   它们不会改变带权路径长度，只用于保证每次都能恰好合并 k 个节点。
 *
 * 时间复杂度：O(n log n)
 * 空间复杂度：O(n)
 */

int main() {
    int n, k;
    cin >> n >> k;

    // 堆中节点为 {子树权值之和, 子树内叶子的最大深度}。
    // pair 按字典序比较：先取权值较小者；权值相同时，取最大深度较小者。
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    for (int i = 0; i < n; i++) {
        long long data;
        cin >> data;
        // 每种单词最初对应一个叶子，尚未发生合并，因此深度为 0。
        pq.emplace(data, 0);
    }

    // 补入最少数量的零权虚拟叶子，使叶子总数满足完整 k 叉树的条件。
    for (int i = 0; i < (k - 1 - (n - 1) % (k - 1)) % (k - 1); i++) pq.emplace(0, 0);

    // sum 记录最终编码的最小总长度，即 Huffman 树的带权路径长度。
    long long sum = 0;
    while (pq.size() > 1) {
        // w 为本次选中子树的权值和，d 为这些子树中的最大深度。
        long long w = 0, d = 0;
        for (int i = 0; i < k; i++) {
            auto tmp = pq.top();
            pq.pop();
            w += tmp.first;
            d = max(d, 0ll + tmp.second);
        }

        // 本次合并使选中子树的所有叶子深度加 1，总代价因此增加 w。
        sum += w;
        // 合并后的父树权值为 w，最大深度为 d + 1。
        pq.emplace(w, d + 1);
    }

    cout << sum << endl;
    // 堆中最后一棵树的最大深度，就是最优方案中最长编码的最短长度。
    cout << pq.top().second << endl;
    return 0;
}
