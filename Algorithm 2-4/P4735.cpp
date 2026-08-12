/**
 * Luogu P4735 - 最大异或和
 *
 * 问题描述：
 *   维护一个只在末尾追加元素的非负整数序列。对于每次询问 Q l r x，
 *   在 l <= p <= r 中选择一个位置 p，使 a[p] ^ a[p+1] ^ ... ^ a[N] ^ x 最大。
 *
 * 前缀异或转换：
 *   定义 prefix[i] = a[1] ^ a[2] ^ ... ^ a[i]，并令 prefix[0] = 0，则
 *
 *     a[p] ^ a[p+1] ^ ... ^ a[N] ^ x
 *     = prefix[N] ^ prefix[p - 1] ^ x。
 *
 *   一次询问中 prefix[N] ^ x 是固定值；由于 l <= p <= r，需要在
 *   prefix[l - 1...r - 1] 中寻找与该固定值异或后最大的元素。
 *
 * 代码保留两种解法：
 *   - solve1：直接枚举 p，单次询问为 O(r - l + 1)，保留作为直观实现和对拍基准；
 *   - solve2：可持久化 01 Trie，每次追加和询问均只处理固定的 24 个二进制位，
 *     main 实际调用此版本。
 *
 * solve2 的版本含义：
 *   root[i] 保存 prefix[0], prefix[1], ..., prefix[i] 构成的 Trie。
 *   每个 Trie 节点的 latest 记录经过该节点的最大 prefix 下标。
 *   查询使用 root[r - 1]，因此其中所有下标均不超过 r - 1；再检查某个分支的
 *   latest 是否不小于 l - 1，即可判断该分支中是否存在合法下标。
 *
 * 复杂度分析：
 *   设二进制位数 B = 24，最终序列长度为 K。
 *   - solve1：单次询问 O(r - l + 1)，空间复杂度 O(K)；
 *   - solve2：初始化 O(NB)，每次追加或询问 O(B)，空间复杂度 O(KB)。
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// 直接枚举版本：逻辑直观，保留用于理解前缀异或转换及与 solve2 对拍。
void solve1() {
    int N, M;
    cin >> N >> M;

    // v 保存原序列，prefix[i] 保存 a[1...i] 的异或值。
    vector v(N + 1, 0), prefix(N + 1, 0);
    for (int i = 1; i <= N; i++) {
        cin >> v[i];
        prefix[i] = prefix[i - 1] ^ v[i];
    }

    for (int i = 1; i <= M; i++) {
        char c;
        cin >> c;
        if (c == 'A') {
            int x;
            cin >> x;

            // 追加原元素，同时由上一个前缀异或值得到新的 prefix。
            v.push_back(x);
            prefix.push_back(prefix.back() ^ x);
        } else {
            int l, r, x;
            cin >> l >> r >> x;

            // base 初始对应 p = l 时的答案。
            int base = prefix.back() ^ prefix[l - 1] ^ x, res = base;
            for (int j = l; j < r; j++) {
                // 异或掉 a[j] 后，base 依次对应 p = j + 1 的答案。
                base ^= v[j];
                res = max(res, base);
            }
            cout << res << '\n';
        }
    }
}

void solve2();

// 题目中的数小于 2^24，最高需要处理 0-based 的第 23 位。
constexpr int MAX_BIT = 23;

// 初始元素和追加操作均不超过 3 * 10^5，额外保留 prefix[0]。
constexpr int MAX_PREFIX_COUNT = 600005;

// 每插入一个 prefix，需要复制一个根节点和 24 个二进制位节点，共 25 个节点。
constexpr int MAX_TRIE_NODE_COUNT = MAX_PREFIX_COUNT * (MAX_BIT + 2);

/**
 * 可持久化 01 Trie。
 *
 * 插入一个新 prefix 时，只复制根节点到对应叶节点的路径；没有发生变化的分支
 * 继续与旧版本共享。这样可以为每个 prefix 下标保存一个 Trie 版本。
 */
class PersistentBinaryTrie {
    struct Node {
        // child[0] 和 child[1] 分别指向下一位为 0、1 的节点编号。
        int child[2];

        // 经过当前节点的最大 prefix 下标，用于判断分支中是否有下标达到左边界。
        int latest;
    };

    // 使用连续数组保存节点，避免大量动态内存分配。
    Node nodes[MAX_TRIE_NODE_COUNT];
    int nodeCount;

    // 复制旧节点并返回新节点编号；旧版本的节点不会被修改。
    int cloneNode(int source) {
        int target = ++nodeCount;
        nodes[target] = nodes[source];
        return target;
    }

public:
    PersistentBinaryTrie() : nodeCount(0) {
        // 0 号节点表示不存在的空节点，它不包含任何合法 prefix 下标。
        nodes[0].child[0] = nodes[0].child[1] = 0;
        nodes[0].latest = -1;
    }

    /**
     * 在 previousRoot 对应的版本中插入 value，返回新版本的根节点。
     * index 是 value 在 prefix 数组中的下标，也是沿途新节点的最新下标。
     */
    int insert(int previousRoot, int value, int index) {
        // 每个版本拥有独立的根节点。
        int newRoot = cloneNode(previousRoot);
        nodes[newRoot].latest = index;

        int previous = previousRoot;
        int current = newRoot;

        for (int bit = MAX_BIT; bit >= 0; bit--) {
            int digit = value >> bit & 1;

            // 只复制 value 实际经过的分支；另一分支已由 cloneNode 从旧节点继承。
            int previousChild = nodes[previous].child[digit];
            int newChild = cloneNode(previousChild);
            nodes[newChild].latest = index;
            nodes[current].child[digit] = newChild;

            previous = previousChild;
            current = newChild;
        }

        return newRoot;
    }

    /**
     * 在 versionRoot 对应的版本中，寻找下标不小于 lowerIndex 的 prefix，
     * 使它与 value 的异或值最大，并返回该最大异或值。
     */
    int query(int versionRoot, int lowerIndex, int value) const {
        int node = versionRoot;
        int result = 0;

        for (int bit = MAX_BIT; bit >= 0; bit--) {
            int digit = value >> bit & 1;

            // 异或的当前位要得到 1，应优先选择与 value 当前位相反的分支。
            int wanted = digit ^ 1;
            int wantedChild = nodes[node].child[wanted];

            // versionRoot 已限制右边界；latest 再负责检查左边界。
            if (wantedChild != 0 && nodes[wantedChild].latest >= lowerIndex) {
                result |= 1 << bit;
                node = wantedChild;
            } else {
                // 相反分支没有合法下标，只能进入与 value 当前位相同的分支。
                node = nodes[node].child[digit];
            }
        }

        return result;
    }
};

// 可持久化 01 Trie 版本：main 实际调用的高效实现。
void solve2() {
    int N, M;
    cin >> N >> M;

    // Trie 节点数组较大，使用静态存储区，避免占用函数调用栈。
    static PersistentBinaryTrie trie;

    vector<int> prefix;
    vector<int> root;
    prefix.reserve(N + M + 1);
    root.reserve(N + M + 1);

    // prefix[0] = 0 必须参与查询，因为 p = 1 时需要使用 prefix[p - 1]。
    prefix.push_back(0);
    root.push_back(trie.insert(0, prefix[0], 0));

    // root[i] 在 root[i - 1] 的基础上插入 prefix[i]，保存 prefix[0...i]。
    for (int i = 1; i <= N; i++) {
        int x;
        cin >> x;
        prefix.push_back(prefix.back() ^ x);
        root.push_back(trie.insert(root.back(), prefix.back(), i));
    }

    for (int i = 1; i <= M; i++) {
        char c;
        cin >> c;
        if (c == 'A') {
            int x;
            cin >> x;

            // prefix.size() 在 push_back 前恰好等于新 prefix 的下标。
            int newIndex = static_cast<int>(prefix.size());
            prefix.push_back(prefix.back() ^ x);
            root.push_back(trie.insert(root.back(), prefix.back(), newIndex));
        } else {
            int l, r, x;
            cin >> l >> r >> x;

            // prefix.back() ^ x 是固定查询值；合法候选下标为 [l - 1, r - 1]。
            // root[r - 1] 限制候选不超过 r - 1，query 的 lowerIndex 限制其不小于 l - 1。
            int target = prefix.back() ^ x;
            cout << trie.query(root[r - 1], l - 1, target) << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 使用满足最大数据范围的可持久化 01 Trie 实现。
    solve2();
    return 0;
}
