/**
 * Luogu P3373 - 【模板】线段树 2
 *
 * 问题描述：
 *   维护一个长度为 n 的数列，支持将闭区间 [x,y] 内的每个元素乘以 k、将闭区间
 *   [x,y] 内的每个元素增加 k，以及查询闭区间 [x,y] 内所有元素的和。所有计算
 *   结果均对给定模数 base 取模。
 *
 * 解法：带乘法和加法懒标记的指针线段树
 *   每个节点维护一段闭区间、该区间当前的元素和 sum，以及尚未下传给儿子的乘法
 *   标记 mul 和加法标记 add。两个标记共同表示仿射变换：
 *       x -> x * mul + add (mod base)
 *   标记的单位状态为 mul=1、add=0；标记对当前区间的影响已经计入 sum。
 *
 *   若要在节点原有标记之后继续施加 x -> x*k+b，则区间和与标记按以下方式合并：
 *       sum <- sum*k + b*区间长度
 *       mul <- mul*k
 *       add <- add*k + b
 *   区间乘法对应 k=乘数、b=0，区间加法对应 k=1、b=加数。统一由 applyTag
 *   完成这两类修改，可以保持多次乘法、加法操作的先后顺序。
 *
 *   修改区间完整覆盖节点时，只更新该节点的 sum 和懒标记；部分覆盖时，先将标记
 *   下传给两个儿子，再根据中点递归处理相交的子区间。查询采用相同的区间拆分方式。
 *
 * 复杂度分析：
 *   - 建树：O(n)
 *   - 区间乘法、区间加法、区间查询：O(log n)
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <vector>
using namespace std;

class Node {
public:
    // sum 是当前区间和；add、mul 组成尚未下传给儿子的仿射变换。
    long long sum, add, mul;
    // 当前节点维护原数组中的闭区间 [bg,ed]。
    int bg, ed;
    // l、r 分别指向当前区间的左半部分和右半部分。
    Node *l, *r;

    Node(long long sum, int bg, int ed, Node *l, Node *r)
        // 新节点还没有待下传修改，因此加法标记为 0、乘法标记为 1。
        : sum(sum), add(0), mul(1), bg(bg), ed(ed), l(l), r(r) {}
};

// 根节点维护原数组区间 [0,n-1]；base 是所有运算使用的模数。
Node *root;
long long base;

// 递归建立维护闭区间 [l,r] 的节点。
Node *buildNode(vector<long long> &v, int l, int r) {
    if (l == r) {
        // 叶节点只对应一个元素，存入线段树前先对模数取模。
        return new Node(v[l] % base, l, r, nullptr, nullptr);
    }
    int mid = (l + r) / 2;
    auto lNode = buildNode(v, l, mid);
    auto rNode = buildNode(v, mid + 1, r);
    // 父节点的区间和等于两个儿子的区间和之和。
    return new Node((lNode->sum + rNode->sum) % base, l, r, lNode, rNode);
}

void build(vector<long long> &v, int n) {
    root = buildNode(v, 0, n - 1);
}

// 返回当前节点所维护闭区间中的元素个数。
int length(Node *node) {
    return node->ed - node->bg + 1;
}

// 对当前节点整体施加 x -> x*mul+add，并将该变换合并到原有懒标记之后。
void applyTag(Node *node, long long mul, long long add) {
    mul %= base;
    add %= base;

    // 区间内每个元素先乘 mul，再增加 add。
    node->sum = (
        node->sum * mul % base +
        add * length(node) % base
    ) % base;

    // 原标记为 x -> x*node->mul+node->add，继续施加新变换后得到：
    // x -> x*(node->mul*mul) + node->add*mul + add。
    node->mul = node->mul * mul % base;
    node->add = (node->add * mul % base + add) % base;
}

// 将当前节点尚未下传的乘法、加法标记交给两个儿子。
void update(Node *node) {
    if (node->l == nullptr) {
        // 叶节点没有儿子，无需继续下传。
        return;
    }
    if (node->mul == 1 && node->add == 0) {
        // 当前标记为单位变换，没有需要下传的修改。
        return;
    }

    // 两个儿子按照同样的先后顺序接收当前节点积累的变换。
    applyTag(node->l, node->mul, node->add);
    applyTag(node->r, node->mul, node->add);

    // 标记已经交给儿子，当前节点恢复单位标记。
    node->mul = 1;
    node->add = 0;
}

// 将当前节点范围内的闭区间 [l,r] 中每个元素增加 add。
void increaseNode(Node *node, int l, int r, long long add) {
    if (l == node->bg && r == node->ed) {
        // 修改区间完整覆盖当前节点，对应仿射变换 x -> x+add。
        applyTag(node, 1, add);
        return;
    }
    // 即将访问儿子时，先保证儿子已经包含当前节点此前积累的修改。
    update(node);
    int mid = (node->bg + node->ed) / 2;
    if (r <= mid) {
        // 修改区间全部位于左子树。
        increaseNode(node->l, l, r, add);
    } else if (l > mid) {
        // 修改区间全部位于右子树。
        increaseNode(node->r, l, r, add);
    } else {
        // 修改区间跨过中点，将其拆成左右两部分分别处理。
        increaseNode(node->l, l, mid, add);
        increaseNode(node->r, mid + 1, r, add);
    }
    // 子树修改完成后，重新计算当前节点维护的区间和。
    node->sum = (node->l->sum + node->r->sum) % base;
}

void increase(int l, int r, long long add) {
    increaseNode(root, l, r, add);
}

// 将当前节点范围内的闭区间 [l,r] 中每个元素乘以 mul。
void multiplyNode(Node *node, int l, int r, long long mul) {
    if (l == node->bg && r == node->ed) {
        // 修改区间完整覆盖当前节点，对应仿射变换 x -> x*mul。
        applyTag(node, mul, 0);
        return;
    }
    // 部分覆盖时先下传已有标记，再继续访问儿子。
    update(node);
    int mid = (node->bg + node->ed) / 2;
    if (r <= mid) {
        // 修改区间全部位于左子树。
        multiplyNode(node->l, l, r, mul);
    } else if (l > mid) {
        // 修改区间全部位于右子树。
        multiplyNode(node->r, l, r, mul);
    } else {
        // 修改区间跨过中点，将其拆成左右两部分分别处理。
        multiplyNode(node->l, l, mid, mul);
        multiplyNode(node->r, mid + 1, r, mul);
    }
    // 子树修改完成后，重新计算当前节点维护的区间和。
    node->sum = (node->l->sum + node->r->sum) % base;
}

void multiply(int l, int r, long long mul) {
    multiplyNode(root, l, r, mul);
}

// 查询当前节点范围内闭区间 [l,r] 的元素和。
long long queryNode(Node *node, int l, int r) {
    if (l == node->bg && r == node->ed) {
        // 查询区间恰好等于当前节点负责的区间，可以直接使用已有区间和。
        return node->sum;
    }
    // 查询需要进入儿子时，先将当前节点的懒标记下传。
    update(node);
    int mid = (node->bg + node->ed) / 2;
    if (r <= mid) {
        // 查询区间全部位于左子树。
        return queryNode(node->l, l, r);
    }
    if (l > mid) {
        // 查询区间全部位于右子树。
        return queryNode(node->r, l, r);
    }
    // 查询区间跨过中点，答案等于左右两部分区间和之和。
    return (queryNode(node->l, l, mid) + queryNode(node->r, mid + 1, r)) % base;
}

long long query(int l, int r) {
    return queryNode(root, l, r);
}

int main() {
    int n, q;
    // n 是数列长度，q 是操作数量，base 是题目给定的模数。
    cin >> n >> q >> base;
    vector<long long> v(n, 0);
    for (int i = 0; i < n; i++) cin >> v[i];
    build(v, n);

    for (int i = 0; i < q; i++) {
        int opt;
        cin >> opt;
        if (opt == 1) {
            int x, y;
            long long k;
            cin >> x >> y >> k;
            // 操作 1：将 1-based 闭区间 [x,y] 转换为 0-based 后整体乘 k。
            multiply(x - 1, y - 1, k);
        } else if (opt == 2) {
            int x, y;
            long long k;
            cin >> x >> y >> k;
            // 操作 2：将 1-based 闭区间 [x,y] 转换为 0-based 后整体增加 k。
            increase(x - 1, y - 1, k);
        } else {
            int x, y;
            cin >> x >> y;
            // 操作 3：查询 1-based 闭区间 [x,y] 的元素和。
            cout << query(x - 1, y - 1) << '\n';
        }
    }
    return 0;
}
