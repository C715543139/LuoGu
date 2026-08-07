/**
 * UVA 11572 - Unique Snowflakes
 *
 * 问题描述：
 *   在雪花编号序列中选择一段连续区间，使其中所有编号互不相同，
 *   求这样的区间最大长度。
 *
 * 解法：滑动窗口 + 哈希集合
 *   1. 窗口 [p, q) 表示当前连续区间，集合 s 保存窗口内所有编号。
 *   2. 右指针 q 向右扩展。若新编号未出现过，直接加入集合。
 *   3. 若新编号已在集合中，当前窗口已无法继续扩展：先更新答案，
 *      再右移左指针 p 并删除元素，直到旧的重复编号离开窗口。
 *   4. 每个元素至多加入集合、删除集合各一次。
 *
 * 复杂度分析：
 *   - 时间复杂度：期望 O(n)，哈希集合操作期望为 O(1)
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读入测试数据组数
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        // 读入当前测试数据的雪花编号序列
        vector v(n, 0);
        for (int i = 0; i < n; i++) cin >> v[i];

        // s 保存当前窗口 [p, q) 内的编号，窗口中始终不含重复元素
        unordered_set<int> s;
        // p、q：窗口左右端点；count：目前找到的最长合法窗口长度
        int p = 0, q = 0, count = 0;
        while (q < n) {
            if (s.count(v[q])) {
                // v[q] 重复，先记录加入它之前的合法窗口长度
                count = max(count, q - p);
                // 移除左端元素，直到旧的 v[q] 被移出窗口
                while (s.count(v[q]) && p < q) s.erase(v[p++]);
            }
            // 此时 v[q] 不在窗口内，可以安全加入
            s.insert(v[q]);
            q++;
        }
        // 处理最长合法窗口延伸到序列末尾的情况
        count = max(count, q - p);
        // 输出当前测试数据的答案
        cout << count << '\n';
    }
    return 0;
}
