/**
 * Luogu P4782 - 【模板】2-SAT
 *
 * 问题描述：
 *   有 n 个布尔变量，每条限制形如 (x_i=a) OR (x_j=b)。判断是否存在一组赋值
 *   满足全部限制；若存在，还要构造任意一组可行解。
 *
 * 解法：蕴含图 + Tarjan 强连通分量
 *   将每个变量拆成两个文字节点：i 表示 x_i=0，i+n 表示 x_i=1。
 *   对于条件 A OR B，使用等价关系
 *       A OR B <=> (NOT A -> B) AND (NOT B -> A)
 *   建立两条蕴含边。若某个变量的真假文字位于同一强连通分量，两者会相互推出，
 *   问题无解；否则根据缩点 DAG 的逆拓扑顺序即可构造一组合法赋值。
 *
 *   本实现采用迭代 Tarjan，避免 2n 个文字节点形成长链时递归栈溢出。SCC 在弹栈时
 *   依次编号，编号较小的分量在缩点 DAG 中更靠近汇点，因此令
 *       x_i = (scc[x_i=0] > scc[x_i=1])。
 *
 * 复杂度分析：
 *   蕴含图包含 2n 个节点、2m 条边：
 *   - 时间复杂度：O(n+m)
 *   - 空间复杂度：O(n+m)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// args[i] 保存变量 x_i 的最终取值。
vector<int> args;

// 使用前向星保存蕴含图，避免为最多 2n 个节点分别创建动态邻接数组。
// head[u] 是 u 的第一条出边编号；to[e] 和 nextEdge[e] 分别是边 e 的终点和下一条边。
vector<int> head, to, nextEdge;

// 添加一条有向蕴含边 u->v。
void addEdge(int u, int v) {
    to.push_back(v);
    nextEdge.push_back(head[u]);
    head[u] = static_cast<int>(to.size()) - 1;
}

// 将文字“x_i=value”映射到蕴含图节点。
// [1,n] 表示取值 0，[n+1,2n] 表示取值 1。
int literal(int i, int value, int n) {
    return i + value * n;
}

// dfn[u] 是访问时间；low[u] 是 u 能到达的栈内节点中的最小时间戳；
// scc[u] 是 u 所属的强连通分量编号。
vector<int> dfn, low, scc;
// parent[u] 是迭代 DFS 树中 u 的父节点；edgeCursor[u] 是 u 下一条待检查的出边。
vector<int> parent, edgeCursor;
// tarjanStack 保存尚未划入 SCC 的节点；dfsStack 模拟递归调用栈。
vector<int> tarjanStack, dfsStack;
// processing[u] 表示 u 当前是否仍在 Tarjan 栈中。
vector<char> processing;
int timer = 0, sccCount = 0;

// 对包含 vertexCount 个节点的蕴含图执行迭代 Tarjan。
void tarjan(int vertexCount) {
    dfn.resize(vertexCount + 1, 0);
    low.resize(vertexCount + 1, 0);
    scc.resize(vertexCount + 1, 0);
    parent.resize(vertexCount + 1, 0);
    edgeCursor.resize(vertexCount + 1, -1);
    processing.resize(vertexCount + 1, false);
    tarjanStack.reserve(vertexCount);
    dfsStack.reserve(vertexCount);

    // 蕴含图不一定连通，从每个尚未访问的文字节点启动一次 DFS。
    for (int start = 1; start <= vertexCount; ++start) {
        if (dfn[start] != 0) {
            continue;
        }

        // 模拟递归函数首次进入 start。
        dfn[start] = low[start] = ++timer;
        parent[start] = 0;
        edgeCursor[start] = head[start];
        processing[start] = true;
        tarjanStack.push_back(start);
        dfsStack.push_back(start);

        while (!dfsStack.empty()) {
            int u = dfsStack.back();
            int &edge = edgeCursor[u];

            if (edge != -1) {
                // 取出当前待处理边，并提前将游标移到 u 的下一条出边。
                int currentEdge = edge;
                edge = nextEdge[currentEdge];
                int v = to[currentEdge];

                if (dfn[v] == 0) {
                    // 模拟递归进入尚未访问的子节点 v。
                    parent[v] = u;
                    dfn[v] = low[v] = ++timer;
                    edgeCursor[v] = head[v];
                    processing[v] = true;
                    tarjanStack.push_back(v);
                    dfsStack.push_back(v);
                } else if (processing[v]) {
                    // v 已访问且仍在 Tarjan 栈中，利用 u->v 更新 low[u]。
                    low[u] = min(low[u], dfn[v]);
                }
                // 已经出栈的 v 所属 SCC 已确定，不参与 low[u] 的更新。
                continue;
            }

            // u 的全部出边已经处理完毕，模拟递归函数从 u 返回。
            dfsStack.pop_back();
            if (parent[u] != 0) {
                low[parent[u]] = min(low[parent[u]], low[u]);
            }

            // u 是一个 SCC 的根，弹出从栈顶到 u 的所有节点。
            if (dfn[u] == low[u]) {
                ++sccCount;
                while (true) {
                    int top = tarjanStack.back();
                    tarjanStack.pop_back();
                    processing[top] = false;
                    scc[top] = sccCount;
                    if (top == u) {
                        break;
                    }
                }
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    const int vertexCount = 2 * n;
    head.resize(vertexCount + 1, -1);
    // 每条限制恰好产生两条蕴含边，预留空间可以避免反复扩容。
    to.reserve(2 * m);
    nextEdge.reserve(2 * m);

    for (int k = 0; k < m; ++k) {
        int i, a, j, b;
        cin >> i >> a >> j >> b;

        int A = literal(i, a, n);
        int notA = literal(i, a ^ 1, n);
        int B = literal(j, b, n);
        int notB = literal(j, b ^ 1, n);

        // (A OR B) 等价于 (NOT A -> B) AND (NOT B -> A)。
        addEdge(notA, B);
        addEdge(notB, A);
    }

    tarjan(vertexCount);

    args.resize(n + 1, -1);
    for (int i = 1; i <= n; ++i) {
        int falseNode = literal(i, 0, n);
        int trueNode = literal(i, 1, n);

        // 真假文字能够相互推出时，无论如何赋值都会产生矛盾。
        if (scc[falseNode] == scc[trueNode]) {
            cout << "IMPOSSIBLE\n";
            return 0;
        }

        // SCC 按 Tarjan 弹栈顺序编号，选择编号较小的文字成立。
        args[i] = scc[falseNode] > scc[trueNode];
    }

    cout << "POSSIBLE\n";
    for (int i = 1; i <= n; ++i) {
        cout << args[i] << ' ';
    }
    cout << '\n';
    return 0;
}
