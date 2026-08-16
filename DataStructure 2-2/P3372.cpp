/**
 * Luogu P3372 - 【模板】线段树 1
 *
 * 问题描述：
 *   维护一个长度为 n 的数列，支持将闭区间 [x,y] 内的每个元素增加 k，以及查询
 *   闭区间 [x,y] 内所有元素的和。
 *
 * 解法：带懒标记的指针线段树
 *   每个节点维护一段闭区间、该区间当前的元素和 val，以及尚未下传给儿子的区间
 *   加法标记 add。add 对当前区间产生的影响已经计入 val，因此查询完整覆盖的节点
 *   时可以直接返回 val。
 *
 *   区间修改完整覆盖某个节点时，只更新该节点的 val 和 add，无需继续访问叶节点。
 *   修改或查询只覆盖当前节点的一部分时，先调用 update 将 add 交给两个儿子，再根据
 *   中点进入左子树、右子树或同时进入两棵子树。修改完成后，用两个儿子的区间和重新
 *   计算父节点的 val。
 *
 * 复杂度分析：
 *   - 建树：O(n)
 *   - 区间增加：O(log n)
 *   - 区间查询：O(log n)
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <vector>
using std::ios, std::cin,std::cout, std::vector;

class Node {
public:
    // 当前节点维护原数组中的闭区间 [left,right]。
    int left, right;
    // val 是当前区间和；add 是尚未下传给儿子的每个元素增量。
    long long val, add;
    // l、r 分别指向当前区间的左半部分和右半部分。
    Node *l, *r;

    Node(int left, int right, long long val, Node *l, Node *r)
        : left(left), right(right), val(val), add(0), l(l), r(r) {}
};

// 整棵线段树的根节点，维护原数组区间 [0,n-1]。
Node *root;

// 递归建立维护闭区间 [left,right] 的节点。
Node *buildNode(vector<long long> &v, int left, int right) {
    if (left == right) {
        // 叶节点只对应一个原数组元素，其区间和就是该元素的值。
        return new Node(left, right, v[left], nullptr, nullptr);
    }
    int mid = (left + right) / 2;
    Node *l = buildNode(v, left, mid);
    Node *r = buildNode(v, mid + 1, right);
    // 父节点的区间和等于两个儿子的区间和之和。
    return new Node(left, right, l->val + r->val, l, r);
}

void build(vector<long long> &v, int n) {
    root = buildNode(v, 0, n - 1);
}

// 将当前节点尚未下传的区间加法标记交给两个儿子。
void update(Node *node) {
    if (node->add == 0 || node->l == nullptr) {
        // 没有懒标记，或者当前节点已经是叶节点，不需要继续下传。
        return;
    }

    // add 表示区间内每个元素都要增加的值，因此对子节点区间和的影响为
    // add 乘以子节点负责的元素个数。
    node->l->val += node->add * (node->l->right - node->l->left + 1);
    node->l->add += node->add;
    node->r->val += node->add * (node->r->right - node->r->left + 1);
    node->r->add += node->add;
    // 标记已经交给两个儿子，当前节点不再保留待下传内容。
    node->add = 0;
}

// 将当前节点范围内的闭区间 [l,r] 中每个元素增加 val。
void increaseNode(Node *node, int l, int r, long long val) {
    if (l == node->left && r == node->right) {
        // 修改区间完整覆盖当前节点：直接更新区间和并留下懒标记。
        node->val += val * (r - l + 1);
        node->add += val;
        return;
    }

    // 即将访问儿子时，先保证儿子已经包含当前节点此前积累的修改。
    update(node);
    int mid = (node->left + node->right) / 2;
    if (r <= mid) {
        // 修改区间全部位于左子树。
        increaseNode(node->l, l, r, val);
    } else if (l > mid) {
        // 修改区间全部位于右子树。
        increaseNode(node->r, l, r, val);
    } else {
        // 修改区间跨过中点，将其拆成左右两部分分别处理。
        increaseNode(node->l, l, mid, val);
        increaseNode(node->r, mid + 1, r, val);
    }

    // 子树修改完成后，重新计算当前节点维护的区间和。
    node->val = node->l->val + node->r->val;
}

void increase(int x, int y, long long k) {
    // 题目下标从 1 开始，线段树内部使用从 0 开始的下标。
    x--, y--;
    increaseNode(root, x, y, k);
}

// 查询当前节点范围内闭区间 [l,r] 的元素和。
long long queryNode(Node *node, int l, int r) {
    if (l == node->left && r == node->right) {
        // 查询区间恰好等于当前节点负责的区间，可以直接使用已有区间和。
        return node->val;
    }
    // 查询需要进入儿子时，先将当前节点的懒标记下传。
    update(node);
    int mid = (node->left + node->right) / 2;
    if (r <= mid) {
        // 查询区间全部位于左子树。
        return queryNode(node->l, l, r);
    }
    if (l > mid) {
        // 查询区间全部位于右子树。
        return queryNode(node->r, l, r);
    }
    // 查询区间跨过中点，答案等于左右两部分的区间和之和。
    return queryNode(node->l, l, mid) + queryNode(node->r, mid + 1, r);
}

long long query(int x, int y) {
    // 将题目的 1-based 闭区间转换为树中的 0-based 闭区间。
    x--, y--;
    return queryNode(root, x, y);
}

int main() {
    // n、m 可达 10^5，关闭同步并解除绑定以加快输入输出。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<long long> v(n);
    for (int i = 0; i < n; i++) cin >> v[i];
    build(v, n);

    for (int i = 0; i < m; i++) {
        int opt;
        cin >> opt;
        if (opt == 1) {
            int x, y;
            long long k;
            cin >> x >> y >> k;
            // 操作 1：将闭区间 [x,y] 内的每个元素增加 k。
            increase(x, y, k);
        } else {
            int x, y;
            cin >> x >> y;
            // 操作 2：输出闭区间 [x,y] 的元素和。
            cout << query(x, y) << '\n';
        }
    }
    return 0;
}
