/**
 * Luogu P3374 - 【模板】树状数组 1
 *
 * 问题描述：
 *   维护一个长度为 n 的数列，支持两种操作：将第 x 个数增加 k，以及查询闭区间
 *   [x,y] 内所有元素的和。
 *
 * 解法：线段树
 *   本文件保留了两种线段树实现：
 *   1. solve1 使用指针连接各个节点。每个节点记录所负责区间的左右端点和区间和；
 *      单点增加时沿根节点到叶节点的路径更新，区间查询时根据中点递归查找。
 *   2. solve2 将线段树按层存入二维数组 vt。vt[i][j] 表示第 i 层的第 j 个节点，
 *      覆盖长度为 2^i 的连续区间。建树时从相邻的两个子节点得到父节点；单点增加
 *      时逐层更新祖先；区间查询时取出左右两端无法向上合并的节点，再将中间的完整
 *      区间映射到上一层继续处理。
 *
 *   main 当前调用 solve2。题目通常使用树状数组求解，本文件的两种线段树实现也能
 *   完成相同的单点增加和区间求和操作。
 *
 * 复杂度分析：
 *   - 建树：O(n)
 *   - 单点增加：O(log n)
 *   - 区间查询：O(log n)
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <vector>
using namespace std;

// 解法一：使用指针实现递归线段树。
void solve1();

class Tree {
    class Node {
    public:
        // 当前节点维护闭区间 [left,right] 的元素和 sum。
        int sum, left, right;
        // l、r 分别指向当前区间的左半部分和右半部分。
        Node *l, *r;

        Node(int sum, int left, int right) : sum(sum), left(left), right(right), l(nullptr), r(nullptr) {}
        Node(int sum, int left, int right, Node *l, Node *r) : sum(sum), left(left), right(right), l(l), r(r) {}
    };

    Node *root;

    // 递归建立负责闭区间 [left,right] 的节点。
    Node *build(vector<int> &v, int left, int right) {
        if (left == right) {
            // 叶节点只负责一个数组元素。
            return new Node(v[left], left, right);
        }
        if (left > right) {
            // 防御性分支：空区间不建立节点。
            return nullptr;
        }

        int mid = (left + right) / 2;
        Node *l = build(v, left, mid);
        Node *r = build(v, mid + 1, right);
        // 父节点的区间和等于左右子节点的区间和之和。
        return new Node(l->sum + r->sum, left, right, l, r);
    }

    // 将下标 idx 对应的元素增加 k，并更新根节点到该叶节点路径上的区间和。
    void increaseNode(Node *node, int idx, int k) {
        if (node->left == node->right && node->left == idx) {
            node->sum += k;
            return;
        }

        // idx 位于当前区间内，因此当前节点维护的区间和也需要增加 k。
        node->sum += k;
        int mid = (node->left + node->right) / 2;
        if (mid >= idx) {
            // idx 位于左半区间。
            increaseNode(node->l, idx, k);
        } else {
            // idx 位于右半区间。
            increaseNode(node->r, idx, k);
        }
    }

    // 查询当前节点范围内闭区间 [left,right] 的元素和。
    int sumNode(Node *node, int left, int right) {
        if (node->left == left && node->right == right) {
            // 查询区间恰好等于当前节点负责的区间，可以直接返回已有结果。
            return node->sum;
        }
        int mid = (node->left + node->right) / 2;
        if (right <= mid) {
            // 查询区间全部位于左子树。
            return sumNode(node->l, left, right);
        }
        if (left > mid) {
            // 查询区间全部位于右子树。
            return sumNode(node->r, left, right);
        }
        // 查询区间跨过中点，分别查询左右两部分后相加。
        return sumNode(node->l, left, mid) + sumNode(node->r, mid + 1, right);
    }

public:
    Tree(vector<int> &v) {
        root = build(v, 0, v.size() - 1);
    }

    void increase(int x, int k) {
        // 题目下标从 1 开始，树中下标从 0 开始。
        increaseNode(root, x - 1, k);
    }

    int sum(int x, int y) {
        // 将题目的闭区间 [x,y] 转换为 0-based 闭区间。
        return sumNode(root, x - 1, y - 1);
    }
};

void solve1() {
    int n, m;
    cin >> n >> m;
    vector<int> v(n);
    for (int i = 0; i < n; i++) cin >> v[i];
    Tree te(v);
    for (int i = 0; i < m; i++) {
        int opt, x, y;
        cin >> opt >> x >> y;
        if (opt == 1) {
            // 操作 1：将第 x 个数增加 y。
            te.increase(x, y);
        } else {
            // 操作 2：查询闭区间 [x,y] 的元素和。
            cout << te.sum(x, y) << '\n';
        }
    }
}

// 解法二：将线段树的各层分别存入二维数组。
void solve2();

// vt[i][j] 维护第 i 层第 j 个连续区间的和，该区间最多包含 2^i 个原数组元素。
vector<vector<int>> vt;
// p 表示线段树的总层数，vt[0] 是叶节点层。
int p;

void build(vector<int> &v, int n) {
    // 从长度 1 开始不断翻倍，计算从叶节点层到根节点层所需的层数。
    p = 1;
    int temp = 1;
    while (temp < n) {
        temp *= 2;
        p++;
    }
    vt = vector<vector<int>>(p);
    // 第 0 层直接保存原数组，每个节点只负责一个元素。
    vt[0] = v;
    for (int i = 1; i < p; i++) {
        if (vt[i - 1].size() % 2 == 1) {
            // 节点数为奇数时在末尾补 0，使最后一个节点也能与空区间合并。
            vt[i - 1].push_back(0);
        }
        for (int j = 0; j + 1 < static_cast<int>(vt[i - 1].size()); j += 2) {
            // 相邻两个子节点合并成上一层的一个父节点。
            vt[i].push_back(vt[i - 1][j] + vt[i - 1][j + 1]);
        }
    }
}

void increase(int x, int k) {
    // 将 1-based 下标转换为第 0 层的 0-based 节点下标。
    x--;
    for (int i = 0; i < p; i++) {
        // 当前节点及它在每一层中的祖先都包含被修改的元素。
        vt[i][x] += k;
        // 第 x 个子节点所属的父节点编号为 x/2。
        x /= 2;
    }
}

int sum(int x, int y) {
    int ans = 0;
    // 题目的 1-based 闭区间 [x,y] 转换为 0-based 半开区间 [x-1,y)。
    x--;
    for (int i = 0; x < y; i++) {
        if (x % 2 == 1) {
            // x 是一对节点中的右节点，左侧搭档不在查询范围内，需要单独计入。
            ans += vt[i][x];
            x++;
        }
        if (y % 2 == 1) {
            // y 是右侧排除边界；y 为奇数时，节点 y-1 无法与右侧搭档共同上移。
            y--;
            ans += vt[i][y];
        }
        // 处理完两端后，剩余区间由完整的节点对组成，将其映射到上一层。
        x /= 2;
        y /= 2;
    }
    return ans;
}

void solve2() {
    int n, m;
    cin >> n >> m;
    vector<int> v(n);
    for (int i = 0; i < n; i++) cin >> v[i];
    build(v, n);
    for (int i = 0; i < m; i++) {
        int opt, x, y;
        cin >> opt >> x >> y;
        if (opt == 1) {
            // 操作 1：将第 x 个数增加 y。
            increase(x, y);
        } else {
            // 操作 2：查询闭区间 [x,y] 的元素和。
            cout << sum(x, y) << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 当前程序使用按层存储的线段树实现。
    solve2();
    return 0;
}
