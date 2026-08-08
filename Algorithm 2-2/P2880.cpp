/**
 * Luogu P2880 - [USACO07JAN] Balanced Lineup G
 *
 * 问题描述：
 *   给定奶牛高度序列，回答多个区间 [l, r] 的“区间最大值 - 区间最小值”。
 *
 * 解法：线段树
 *   每个节点表示一个连续区间，并保存该区间的最小高度 mn 与最大高度 mx。
 *   - 建树时，将父区间分为左右子区间，再合并子节点的 mn、mx；
 *   - 查询时，完全被查询区间包含的节点可直接返回；
 *   - 部分相交的节点继续递归访问相关子节点，并合并查询结果。
 *
 * 本文件包含两种线段树实现：
 *   - solve1：每个节点由裸指针连接的真实树结构；
 *   - solve2：用数组下标模拟树结构，main 当前调用此版本。
 *
 * 复杂度分析：
 *   - 建树：O(n)
 *   - 单次查询：O(log n)
 *   - 空间复杂度：O(n)
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

/**
 * 解法一：使用 Node* 指针连接父节点和子节点的线段树。
 *
 * 每个 Node 持有它所代表的左右端点，以及对应区间的最小值、最大值。
 */
void solve1() {
    class SegmentTree {
    public:
        // 建立覆盖整个 0-based 高度数组的根节点
        SegmentTree(vector<int> &v) {
            int left = 0, right = v.size() - 1;
            root = build(v, left, right);
        }

        // 查询闭区间 [left, right] 的最小值和最大值
        pair<int, int> find(int left, int right) {
            return findInNode(root, left, right);
        }

    private:
        class Node {
        public:
            // minH、maxH：节点区间 [left, right] 的最小高度和最大高度
            int minH = INT_MAX, maxH = INT_MIN;
            int left, right;
            // 两个子区间；叶子节点的子指针均为 nullptr
            Node *lChild, *rChild;

            Node(int left, int right, int minH, int maxH, Node *lChild, Node *rChild) : minH(minH), maxH(maxH),
                left(left), right(right), lChild(lChild), rChild(rChild) {}
        };

        Node *root;

        // 递归建立表示区间 [left, right] 的节点
        Node *build(vector<int> &v, int left, int right) {
            if (left == right) {
                // 叶子节点只包含一头奶牛
                return new Node(left, right, v[left], v[left], nullptr, nullptr);
            }

            // 划分为左右两个子区间，并由子节点极值合并父节点极值
            Node *lChild = build(v, left, (left + right) / 2);
            Node *rChild = build(v, (left + right) / 2 + 1, right);
            int minH = min(lChild->minH, rChild->minH), maxH = max(lChild->maxH, rChild->maxH);
            return new Node(left, right, minH, maxH, lChild, rChild);
        }

        // 在 node 子树中查询闭区间 [left, right]
        pair<int, int> findInNode(Node *node, int left, int right) {
            if (left == node->left && node->right == right) {
                // 当前查询区间与该节点区间恰好一致，直接使用预处理好的极值
                return {node->minH, node->maxH};
            }

            int mid = (node->left + node->right) / 2;
            if (left > mid) {
                // 查询区间完全落在右子树
                return findInNode(node->rChild, left, right);
            }
            if (right <= mid) {
                // 查询区间完全落在左子树
                return findInNode(node->lChild, left, right);
            }

            // 查询区间跨越中点，分别查询左右子树后合并极值
            auto lResult = findInNode(node->lChild, left, mid);
            auto rResult = findInNode(node->rChild, mid + 1, right);

            return {min(lResult.first, rResult.first), max(lResult.second, rResult.second)};
        }
    };

    int n, q;
    cin >> n >> q;
    // solve1 使用 0-based 下标保存高度
    vector v(n, 0);
    for (int i = 0; i < n; i++) cin >> v[i];
    SegmentTree st(v);
    for (int i = 0; i < q; i++) {
        int left, right;
        cin >> left >> right;
        // 题目输入为 1-based 下标，转换后查询
        auto res = st.find(left - 1, right - 1);
        cout << res.second - res.first << endl;
    }
}

// 解法二：数组线段树的声明，main 调用此版本
void solve2();

// tree[p] 表示数组线段树中编号为 p 的节点，记录区间最小值与最大值
struct Node {
    int mn = INT_MAX;
    int mx = INT_MIN;
};

// height 采用 1-based 下标；tree 的子节点编号为 p*2 与 p*2+1
vector<int> height;
vector<Node> tree;

// 建立表示闭区间 [l, r] 的数组线段树节点 p
void build(int p, int l, int r) {
    if (l == r) {
        // 叶子节点对应一头奶牛
        tree[p].mn = tree[p].mx = height[l];
        return;
    }

    int mid = (l + r) / 2;
    build(p * 2, l, mid);
    build(p * 2 + 1, mid + 1, r);

    // 父节点极值由左右子节点合并得到
    tree[p].mn = min(tree[p * 2].mn, tree[p * 2 + 1].mn);
    tree[p].mx = max(tree[p * 2].mx, tree[p * 2 + 1].mx);
}

// 查询 node p 所表示区间 [l, r] 与查询区间 [ql, qr] 的交集极值
Node query(int p, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        // 当前节点区间完全属于查询区间，直接返回
        return tree[p];
    }

    int mid = (l + r) / 2;
    // 初值便于后续合并左、右查询结果
    Node result;

    if (ql <= mid) {
        // 查询区间与左子树 [l, mid] 有交集
        Node leftResult = query(p * 2, l, mid, ql, qr);
        result.mn = min(result.mn, leftResult.mn);
        result.mx = max(result.mx, leftResult.mx);
    }

    if (qr > mid) {
        // 查询区间与右子树 [mid+1, r] 有交集
        Node rightResult = query(p * 2 + 1, mid + 1, r, ql, qr);
        result.mn = min(result.mn, rightResult.mn);
        result.mx = max(result.mx, rightResult.mx);
    }

    return result;
}

void solve2() {
    int n, q;
    cin >> n >> q;

    // 多开一格，使用与题目一致的 1-based 高度下标
    height.resize(n + 1);
    // 数组线段树最多使用约 4n 个节点
    tree.resize(n * 4 + 5);

    for (int i = 1; i <= n; i++) {
        cin >> height[i];
    }

    build(1, 1, n);

    while (q--) {
        int l, r;
        cin >> l >> r;
        // 防御性处理，保证查询左端点不大于右端点
        if (l > r) swap(l, r);

        // 合并后的结果即为整个 [l, r] 的极值
        Node result = query(1, 1, n, l, r);
        cout << result.mx - result.mn << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 使用数组线段树版本
    solve2();
    return 0;
}
