/**
 * Luogu P3368 - 【模板】树状数组 2
 *
 * 问题描述：
 *   维护一个长度为 n 的数列，支持将闭区间 [x,y] 内的每个元素增加 k，以及查询
 *   第 x 个元素当前的值。
 *
 * 解法：差分数组 + 按层存储的线段树
 *   令差分数组为 d，其中 d[1]=a[1]，d[i]=a[i]-a[i-1]。原数组中的一个元素可以
 *   由差分数组的前缀和恢复：a[x]=d[1]+d[2]+...+d[x]。
 *
 *   将原数组的区间 [x,y] 全部增加 k 时，差分数组只需修改两个位置：
 *   1. d[x] 增加 k，使从 a[x] 开始的所有前缀和增加 k；
 *   2. 若 y<n，则令 d[y+1] 减少 k，抵消 a[y+1] 及其后元素受到的影响。
 *
 *   二维数组 vt 按层保存线段树。vt[0] 保存差分数组，每升高一层就将相邻两个
 *   节点的和合并为一个父节点。这样可以在 O(log n) 时间内修改一个差分值，也能
 *   在 O(log n) 时间内求出 d[1..x] 的和，从而得到原数组第 x 个元素的值。
 *
 * 复杂度分析：
 *   - 建立差分数组及线段树：O(n)
 *   - 区间增加：O(log n)
 *   - 单点查询：O(log n)
 *   - 处理全部操作：O(n + m log n)
 *   - 空间复杂度：O(n)
 *
 * 代码说明：
 *   本文件使用按层线段树维护差分值。当前数值类型为 int；题目只限制单个元素的
 *   绝对值不超过 2^30，相邻元素的差值在极端情况下可能达到 2^31。若需完整覆盖
 *   该边界，应将差分值、区间和及修改量相关类型改为 long long。
 */

#include <iostream>
#include <vector>
using namespace std;

// vt[i][j] 表示第 i 层的第 j 个区间之和；第 i 层的区间长度最多为 2^i。
vector<vector<int>> vt;
// p 是线段树的总层数，vt[0] 为保存差分数组的叶节点层。
int p = 1;

void build(vector<int> &v, int n) {
    // 从一个元素开始不断将容量翻倍，计算从叶节点到根节点所需的层数。
    int temp = 1;
    while (temp < n) {
        temp *= 2;
        p++;
    }
    vt = vector<vector<int>>(p);
    // 第 0 层直接保存差分数组，每个节点只负责一个差分值。
    vt[0] = v;
    for (int i = 1; i < p; ++i) {
        if (vt[i - 1].size() % 2 == 1) {
            // 节点数为奇数时在末尾补 0，使最后一个节点也能参与两两合并。
            vt[i - 1].push_back(0);
        }
        for (int j = 0; j < vt[i - 1].size() - 1; j += 2) {
            // 相邻两个子节点的和构成上一层的一个父节点。
            vt[i].push_back(vt[i - 1][j] + vt[i - 1][j + 1]);
        }
    }
}

// 将差分数组中第 x 个数增加 k。
void increase(int x, int k) {
    // 题目下标从 1 开始，vt 中的节点下标从 0 开始。
    x--;
    for (int i = 0; i < p; ++i) {
        // 当前节点及其每一层祖先维护的区间和都需要增加 k。
        vt[i][x] += k;
        // 第 x 个子节点所属的父节点编号为 x/2。
        x /= 2;
    }
}

// 求差分数组闭区间 [x,y] 的和。
int sum(int x, int y) {
    // 将 1-based 闭区间 [x,y] 转换为 0-based 半开区间 [x-1,y)。
    x--;
    int res = 0;
    for (int i = 0; x < y; ++i) {
        if (x % 2 == 1) {
            // x 是一对节点中的右节点，左侧搭档不在查询范围内，需要单独计入。
            res += vt[i][x];
            x++;
        }
        if (y % 2 == 1) {
            // y 是右侧排除边界；节点 y-1 无法与右侧搭档共同映射到父节点。
            y--;
            res += vt[i][y];
        }
        // 两端处理完毕后，剩余区间由完整节点对组成，可以映射到上一层。
        x /= 2;
        y /= 2;
    }
    return res;
}

int main() {
    // 数据规模可达 5*10^5，关闭同步并解除绑定以加快输入输出。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    // v 随读入过程由原数组转换为差分数组。
    vector v(n, 0);
    for (int i = 0, last = 0; i < n; ++i) {
        cin >> v[i];
        // last 保存前一个原数组元素；两者相减得到当前差分值。
        v[i] -= last;
        // last + 当前差分值等于本次读入的原数组元素，供下一轮使用。
        last += v[i];
    }
    build(v, n);

    for (int i = 1; i <= m; ++i) {
        int opt;
        cin >> opt;
        if (opt == 1) {
            int x, y, k;
            cin >> x >> y >> k;
            // d[x] 增加 k，使原数组从第 x 项开始全部增加 k。
            increase(x, k);
            if (y < n) {
                // d[y+1] 减少 k，将区间增加的影响限制在 [x,y] 内。
                increase(y + 1, -k);
            }
        } else {
            int x;
            cin >> x;
            // a[x] 等于差分数组前 x 项之和。
            cout << sum(1, x) << '\n';
        }
    }
    return 0;
}
