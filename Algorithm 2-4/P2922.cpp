/**
 * Luogu P2922 - [USACO08DEC] Secret Message G
 *
 * 问题描述：
 *   给定若干条二进制消息。对于每条待查询的二进制暗号，统计有多少消息与它匹配。
 *   两个序列匹配，当且仅当其中较短的序列是较长序列的前缀。
 *
 * 解法：二进制 Trie + 前缀计数
 *   将全部消息插入二进制 Trie。每个节点维护两个计数：
 *   - endCount：恰好在当前节点结束的消息数量；
 *   - passCount：经过当前节点的消息数量，即以当前节点对应序列为前缀的消息数量。
 *   使用整数计数可以正确保留内容相同的多条消息。
 *
 *   查询暗号时沿它在 Trie 中的路径前进，答案由两部分组成：
 *   1. 路径上祖先节点的 endCount，表示“消息是暗号的真前缀”；
 *   2. 完整暗号所在节点的 passCount，表示“暗号是消息的前缀”，其中也包含相同消息。
 *   若中途不存在下一条边，只有此前遇到的消息前缀能够匹配，可以直接返回累计结果。
 *
 * 复杂度分析：
 *   设所有消息的总位数为 S，所有暗号的总位数为 Q。
 *   - 时间复杂度：O(S + Q)
 *   - 空间复杂度：O(S)
 */

#include <iostream>
#include <vector>
using namespace std;

// 二进制 Trie：左边表示下一位为 0，右边表示下一位为 1。
class Tree {
    class Node {
    public:
        // 当前节点代表的二进制位；根节点使用 -1 作为占位值。
        int data;
        Node *left, *right;
        // 在当前节点结束的消息数量，用于保留重复消息。
        int endCount;
        // 经过当前节点的消息数量，即以当前节点对应序列为前缀的消息数量。
        int passCount;

        Node(int data, Node *left, Node *right)
            : data(data), left(left), right(right), endCount(0), passCount(0) {}
    };

    Node *root;

public:
    // 根节点不代表消息中的实际二进制位。
    Tree() : root(new Node(-1, nullptr, nullptr)) {}

    // 将一条消息插入 Trie，并更新沿途节点及消息终点的计数。
    void insert(const vector<int> &data) {
        Node *node = root;
        for (const int bit: data) {
            if (bit == 0) {
                // 当前前缀尚无 0 分支时，创建对应节点。
                if (node->left == nullptr) {
                    node->left = new Node(0, nullptr, nullptr);
                }
                node = node->left;
            } else {
                // 当前前缀尚无 1 分支时，创建对应节点。
                if (node->right == nullptr) {
                    node->right = new Node(1, nullptr, nullptr);
                }
                node = node->right;
            }
            // 当前消息经过该节点，因此它属于以该节点对应序列为前缀的消息。
            node->passCount++;
        }

        // 消息在当前节点结束；重复消息会多次累加。
        node->endCount++;
    }

    // 统计与给定暗号匹配的消息数量。
    int solve(const vector<int> &data) {
        Node *node = root;
        int res = 0;

        for (const int bit: data) {
            // 读取下一位前，当前节点对应暗号的一个真前缀；
            // 在这里结束的全部消息均能与暗号匹配。
            res += node->endCount;

            // 继续沿暗号当前位对应的 Trie 分支向下查询。
            Node *next = bit == 0 ? node->left : node->right;
            if (next == nullptr) {
                // Trie 中不存在该前缀，更长的消息无法匹配，只保留此前统计的消息前缀。
                return res;
            }
            node = next;
        }

        // passCount 包含与暗号相同，以及以暗号为前缀的全部消息。
        return res + node->passCount;
    }
};

int main() {
    int M, N;
    cin >> M >> N;

    vector<int> v;
    Tree te;
    // 读入 M 条截获的消息并建立 Trie。
    for (int i = 0; i < M; i++) {
        int b;
        cin >> b;
        for (int j = 0; j < b; j++) {
            int x;
            cin >> x;
            v.push_back(x);
        }
        te.insert(v);
        v.clear();
    }

    // 逐条读入暗号并输出能够与其匹配的消息数量。
    for (int i = 0; i < N; i++) {
        int b;
        cin >> b;
        for (int j = 0; j < b; j++) {
            int x;
            cin >> x;
            v.push_back(x);
        }
        cout << te.solve(v) << '\n';
        v.clear();
    }
    return 0;
}
