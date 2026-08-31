/**
 * Luogu P2014 - [CTSC1997] 选课
 *
 * 问题描述：
 *   有 N 门课程，每门课程有一定学分，部分课程必须先学习其直接先修课。
 *   求选择恰好 M 门课程时能够获得的最大学分。
 *
 * 解法：树形背包动态规划
 *   每门课程至多有一门直接先修课，因此课程依赖关系是一片森林。
 *   添加一个学分为 0 的虚拟节点 0，将所有没有先修课的课程连接到 0，
 *   这样就可以把森林转换成一棵以 0 为根的树。
 *
 *   如果选择某门课程，就必须选择从虚拟根到它之间的所有课程。
 *   因此，在以 node 为根的子树中进行选课时，node 必须被选择。
 *
 *   dfs(node) 返回一个数组 result：
 *     result[j]（j >= 1）表示在 node 的子树中选择恰好 j 个节点，
 *     且已经选择 node 时能够获得的最大学分。
 *     特殊地，result[0] = 0 表示完全跳过 node 的整棵子树，供父节点转移使用。
 *   对于虚拟节点 0，它虽然不是真实课程，但会占用一个 DP 名额，学分为 0。
 *
 *   为了显式表示子树合并过程，使用两个数组：
 *     current[j] 表示已经处理完当前节点的前若干个子树后，选择 j 个节点的最优值；
 *     next[j] 表示再加入当前正在处理的子树后，选择 j 个节点的最优值。
 *
 *   如果 current 中已经选择 i 个节点，从 child 子树中选择 j 个节点，
 *   由于“当前已处理部分”和“child 子树”在树上互不重叠，
 *   合并后选择 i+j 个节点，转移为：
 *     next[i+j] = max(next[i+j], current[i] + childDp[j])。
 *
 *   childDp[0] = 0 表示不选择 child 子树中的任何真实课程，
 *   此时 child 本身也不被选择。这个状态只用于表示“跳过整棵子树”。
 *   由于 current 从 node 自身开始初始化，所以每个有效状态都包含 node，
 *   先修关系也就能够自然满足。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(NM^2)，合并每个子树时枚举两部分选择的节点数量；
 *   - 空间复杂度：O(NM)，保存树结构、子树大小和 DP 数组。
 */

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

const int NEG_INF = -1000000000;

int n, m;

// tree[u] 保存课程 u 的所有后续课程
vector<vector<int>> tree;

// credits[u] 表示课程 u 的学分，虚拟节点 0 的学分为 0
vector<int> credits;

// subtreeSize[u] 表示以 u 为根的子树中包含的节点数
vector<int> subtreeSize;

int main() {
    // 读入课程总数和需要选择的真实课程数量
    cin >> n >> m;

    // 节点 0 是虚拟根节点，真实课程编号为 1 到 n
    tree.assign(n + 1, {});
    credits.assign(n + 1, 0);
    subtreeSize.assign(n + 1, 0);

    // 读入每门课程的直接先修课和学分
    for (int i = 1; i <= n; ++i) {
        int prerequisite, credit;
        cin >> prerequisite >> credit;

        // prerequisite -> i 表示 prerequisite 是 i 的直接先修课
        // prerequisite 为 0 时，i 直接连接到虚拟根节点
        tree[prerequisite].push_back(i);
        credits[i] = credit;
    }

    // 记忆化搜索：返回当前节点子树的完整 DP 数组
    function<vector<int>(int)> dfs = [&](int node) {
        // current[j] 表示目前已经处理的部分选择 j 个节点时的最大学分
        vector current(m + 2, NEG_INF);

        // 选择当前节点本身，作为当前子树 DP 的起点
        current[1] = credits[node];
        int processedSize = 1;

        // 依次合并当前节点的每一棵子树
        for (int child : tree[node]) {
            // 先递归计算 child 子树中选择不同数量节点时的最优值
            vector<int> childDp = dfs(child);

            // next[j] 显式表示加入 child 子树后的临时 DP 结果
            vector next(m + 2, NEG_INF);

            // currentCount：已经处理部分选择的节点数
            for (int currentCount = 1;
                 currentCount <= min(processedSize, m + 1);
                 ++currentCount) {
                // 当前状态不可行时，跳过后续计算
                if (current[currentCount] == NEG_INF) {
                    continue;
                }

                // childCount = 0 表示完全不选择 child 子树
                for (int childCount = 0;
                     childCount <= min(subtreeSize[child], m + 1 - currentCount);
                     ++childCount) {
                    // childDp[childCount] 不可行时，不能参与状态转移
                    if (childDp[childCount] == NEG_INF) {
                        continue;
                    }

                    // 显式合并：已处理部分的 current 与当前子树的 childDp
                    int totalCount = currentCount + childCount;
                    next[totalCount] = max(
                        next[totalCount],
                        current[currentCount] + childDp[childCount]
                    );
                }
            }

            // child 已经合并进来，next 成为下一轮的 current
            current = next;
            processedSize = min(m + 1, processedSize + subtreeSize[child]);
        }

        // 当前子树的节点数量不能超过 m+1，但仍需记录真实大小用于后续合并
        subtreeSize[node] = processedSize;
        // 特殊状态：供父节点表示“不选择当前子树”
        current[0] = 0;
        return current;
    };

    // 虚拟根节点也会占用一个 DP 名额，因此选择 m 门真实课程对应 m+1 个节点
    vector<int> answer = dfs(0);

    // 输出选择 m 门真实课程时的最大学分
    cout << answer[m + 1] << endl;
    return 0;
}
