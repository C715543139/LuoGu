/**
 * Luogu P1481 - 魔族密码
 *
 * 问题描述：
 *   从给定单词中选出尽可能长的序列，使序列中每个单词都是后一个单词的前缀，
 *   求该前缀链最多能包含多少个单词。
 *
 * 解法：Trie + 枚举单词前缀
 *   一条前缀链中的所有单词，都是链尾最长单词的前缀。
 *   因此枚举每个已存单词，逐字符构造它的全部前缀，并在 Trie 中查询这些前缀
 *   是否也是完整单词；命中的前缀数量就是以该单词结尾的最长链长度。
 *
 *   代码保留两种实现：
 *   - solve1：手写 Trie，沿根到叶子的路径统计单词结束节点数量；
 *   - solve2：使用 GNU PBDS Trie，对每个单词枚举并查询全部前缀，main 实际调用此版本。
 *   题目保证输入按字典序排列且没有重复单词，因此一个单词的较短前缀一定先被插入；
 *   solve1 的递归查找依赖这一输入顺序，solve2 的前缀查询不依赖插入顺序。
 *
 * 复杂度分析：
 *   设所有单词的总字符数为 S，最大长度为 L，Trie 节点数为 M（M <= S + 1）。
 *   - solve1：每深入一层最多线性扫描 26 个小写字母对应的子节点，全部插入为
 *     O(26S) = O(S)；DFS 访问每个节点和每条边一次，为 O(M)，因此总时间为 O(S)。
 *     Trie 节点及子节点指针占 O(M) = O(S) 空间，递归栈最深为 O(L)。
 *   - solve2：对每个单词查询其所有前缀，时间复杂度为 O(sum(|word|^2))，
 *     最坏为 O(nL^2)；PBDS Trie 占 O(S) 空间。
 */

#include <iostream>
#include <vector>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;

// 手写 Trie 版本的入口，保留用于和 PBDS 版本对照。
void solve1();

// 手写 Trie：每个节点表示一个字符，isWord 标记从根到当前节点是否组成完整单词。
// children 使用 vector，查找下一字符时需要线性扫描；小写字母字符集使子节点数最多为 26。
class Trie {
    class Node {
    public:
        char c;
        bool isWord;
        vector<Node *> children;
        Node(char c, bool isWord) : c(c), isWord(isWord) {}
    };

    // 沿 word 的字符寻找已有路径，返回最后匹配的节点和第一个未匹配字符的位置。
    // 题目保证字典序插入且无重复：当前单词不会是已插入长单词的前缀，
    // 因而 it 到达 word.end() 时所在节点没有子节点，不会在循环中解引用尾后迭代器。
    pair<Node *, string::const_iterator> searchNode(Node *node, string::const_iterator it) {
        for (auto child: node->children) {
            if (child->c == *it) {
                return searchNode(child, it + 1);
            }
        }
        return {node, it};
    }

    // 从根节点开始查找 word 的最长已有前缀。
    pair<Node *, string::const_iterator> search(const string &word) {
        return searchNode(root, word.begin());
    }

    Node *root;

public:
    Trie() {
        // 根节点不代表实际字符，也不构成单词。
        root = new Node('.', false);
    };

    // 复用已有前缀节点，并为剩余字符创建新节点；末尾节点标记为完整单词。
    void insert(const string &word) {
        auto temp = search(word);
        auto parent = temp.first;
        auto it = temp.second;
        while (it != word.end()) {
            Node *temp = new Node(*it, false);
            parent->children.push_back(temp);
            parent = temp;
            ++it;
        }
        parent->isWord = true;
    }

    // 一条根到叶子的路径对应一组具有前缀关系的单词；res 记录当前路径上的单词数。
    int dfs(int res, Node *node) {
        if (node->children.empty()) {
            // 插入形成的叶节点一定是某个完整单词的结尾。
            return res + 1;
        }

        if (node->isWord) {
            res++;
        }
        int maxRes = res;
        for (auto child: node->children) {
            maxRes = max(maxRes, dfs(res, child));
        }
        return maxRes;
    }

    // 遍历整棵 Trie，求任意根到叶子路径上最多包含多少个完整单词。
    int longest() {
        return dfs(0, root);
    }
};

// 手写 Trie 版本。
void solve1() {
    int n;
    cin >> n;

    Trie te;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        te.insert(s);
    }
    cout << te.longest() << endl;
}

// PBDS Trie 版本：通过完整键查询统计每个单词拥有多少个已存前缀；该逻辑不依赖插入顺序。
void solve2() {
    int n;
    cin >> n;

    // null_type 表示只保存字符串键，不为键附加映射值。
    __gnu_pbds::trie<string, __gnu_pbds::null_type> te;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        te.insert(s);
    }

    int res = 0;
    // 枚举每个已存单词，将它视为一条前缀链中最长的链尾单词。
    for (auto &word: te) {
        string prefix;
        int count = 0;
        for (auto &c: word) {
            // 依次得到长度为 1、2、...、word.length() 的所有前缀。
            prefix += c;

            // find 查询完整键；当前前缀存在时，它可以作为链中的一个单词。
            count += te.find(prefix) != te.end();
        }

        // 保留所有链尾单词对应的最大链长度。
        res = max(res, count);
    }
    cout << res << endl;
}

int main() {
    // 使用 GNU PBDS Trie 实现。
    solve2();
    return 0;
}
