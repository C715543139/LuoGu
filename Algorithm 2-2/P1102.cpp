/**
 * Luogu P1102 - A-B 数对
 *
 * 问题描述：
 *   给定 N 个整数和正整数 C，统计所有满足 A - B = C 的数对数量。
 *   数值相同但所在位置不同的元素视为不同的数对成员。
 *
 * 解法：排序 + 哈希表
 *   1. 将数组按数值升序排序。由于 C > 0，数对中的 B 一定先于 A 被扫描到。
 *   2. mp[target] 记录已经扫描过的数作为 B 时，能够匹配到 target 的次数，
 *      即每读到一个 B，就令 mp[B + C] 加一。
 *   3. 扫描当前值 A 时，mp[A] 就是此前所有满足 A - B = C 的 B 的数量，
 *      将其累加到答案中。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(N log N)，排序为 O(N log N)，哈希表扫描期望为 O(N)
 *   - 空间复杂度：O(N)，用于哈希表
 */

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

int main() {
    int N, c;
    cin >> N >> c;
    // 读入原数列
    vector v(N, 0);
    for (int i = 0; i < N; i++) {
        cin >> v[i];
    }

    // 升序排序，保证较小的 B 会先于对应的 A 被处理
    sort(v.begin(), v.end());
    // 合法数对数最多可达 O(N^2)，使用 long long 保存
    long long count = 0;
    // mp[target]：此前每个 B 贡献的目标值 B + c 的出现次数
    unordered_map<int, int> mp;
    // 依次将 v[i] 作为数对中的 A 处理
    for (int i = 0; i < N; i++) {
        // 所有目标值等于当前 A 的记录，均对应一组合法的 (A, B)
        if (mp.count(v[i])) {
            count += mp[v[i]];
        }
        // 将当前值作为后续数对中的 B，等待值为 v[i] + c 的 A 出现
        mp[v[i] + c]++;
    }
    // 输出满足 A - B = c 的数对总数
    cout << count << endl;
    return 0;
}
