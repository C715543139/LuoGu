/**
 * Luogu P2580 - 于是他错误的点名开始了
 *
 * 问题描述：
 *   给定竞赛组的学生名单，依次判断教练报出的名字：
 *   名字不在名单中时输出 WRONG；第一次点到名单中的学生时输出 OK；
 *   再次点到已经报过的学生时输出 REPEAT。
 *
 * 解法：哈希表 + 状态标记
 *   使用哈希表保存名单中的姓名，并为每个姓名记录一个状态：
 *   - 1 表示该学生尚未被点到；
 *   - 0 表示该学生已经被点到。
 *   查询时先判断姓名是否存在；若存在，再根据状态区分首次点名和重复点名。
 *   首次点名后将状态由 1 改为 0，供后续查询判断。
 *
 * 复杂度分析：
 *   设名单人数为 n，点名次数为 m，姓名最大长度为 L。
 *   - 平均时间复杂度：O((n + m)L)
 *   - 空间复杂度：O(nL)
 */

#include <iostream>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;

int main() {
    int n;
    cin >> n;

    // 姓名作为键，状态 1 表示尚未点到，状态 0 表示已经点到。
    __gnu_pbds::gp_hash_table<string, int> ht;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        // 名单中的每名学生最初都处于“尚未点到”状态。
        ht[s] = 1;
    }

    // 复用 n 保存教练接下来的点名次数。
    cin >> n;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        // 在哈希表中查找当前姓名，平均只需常数次哈希表操作。
        auto it = ht.find(s);
        if (it == ht.end()) {
            // 姓名不在学生名单中。
            cout << "WRONG" << '\n';
        } else if (it->second == 1) {
            // 第一次点到名单中的学生，并将其标记为已经点到。
            cout << "OK" << '\n';
            it->second = 0;
        } else {
            // 该学生此前已经被点到过。
            cout << "REPEAT" << '\n';
        }
    }
    return 0;
}
