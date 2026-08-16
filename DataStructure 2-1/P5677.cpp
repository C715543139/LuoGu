/**
 * Luogu P5677 - [GZOI2017] 配对统计
 *
 * 问题描述：
 *   对于有序配对 (x,y)，若 a[y] 是整个数组中距离 a[x] 最近的值之一，则称它为
 *   好的配对。每次询问闭区间 [l,r] 内包含多少个好的配对，最终输出第 i 次询问
 *   的答案乘以 i 后的总和。
 *
 * 解法：排序预处理好配对 + 离线扫描 + PBDS
 *   1. 将 (a[i],i) 按数值排序。由于所有 a[i] 互不相同，一个元素的最近值只可能
 *      是数值排序后的直接前驱或直接后继。比较左右差值后加入较近者；距离相等时
 *      两者都会与当前元素组成好的配对。因此好的有序配对总数为 O(n)。
 *   2. 对每个有序配对 (x,y)，令 left=min(x,y)、right=max(x,y)，并在 pr 中保存
 *      (right,left)。配对的两个下标都在 [l,r] 内，等价于 right<=r 且 left>=l。
 *      虽然规范化后不再区分方向，但每个有序配对仍各自占据 pr 中的一项，所以
 *      (x,y) 和 (y,x) 同时成立时会保留两次贡献。
 *   3. 将 pr 按 right 升序排列，将询问也按右端点 r 升序离线处理。扫描到询问
 *      [l,r] 时，把所有 right<=r 的配对加入 PBDS；PBDS 按 (left,唯一编号) 排序。
 *      order_of_key({l,-1}) 统计 left<l 的配对，因此用当前总数减去它，就得到同时
 *      满足 right<=r、left>=l 的配对数量。
 *   4. 离线排序会改变询问次序，因此 qr 额外保存原编号，计算最终总和时仍使用
 *      原编号加一作为权重。
 *
 * 复杂度分析：
 *   - 预处理并排序好配对：O(n log n)
 *   - 排序并处理全部询问：O(m log m + (n+m) log n)
 *   - 总时间复杂度：O((n+m) log(n+m))
 *   - 空间复杂度：O(n+m)
 *
 * 代码说明：
 *   当 n=1 时不存在 x!=y 的配对，生成 pr 的循环条件 n>1 会直接跳过循环。
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;

int main() {
    // n、m 最多达到 3*10^5，关闭同步并解除绑定以加快输入输出。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    // v 按原下标保存数值；s 按“数值、原下标”排序，便于寻找全局最近值。
    vector v(n, 0);
    set<pair<int, int>> s;
    for (int i = 0; i < n; i++) {
        cin >> v[i];
        s.insert({v[i], i});
    }

    // pr 中每一项为 (right,left)，表示一个规范化后的有序好配对。
    // 同一对端点可能出现两次，分别对应 (x,y) 和 (y,x) 两个有序配对。
    vector<pair<int, int>> pr;
    pr.reserve(n);
    for (auto it = s.begin(); n > 1 && it != s.end(); ++it) {
        // front、back 将分别移动到当前元素在数值顺序中的前驱和后继。
        auto front = it, back = it;
        ++back;
        if (it == s.begin() && back != s.end()) {
            // 最小值只有右侧候选，其后继一定是它的全局最近值。
            pr.emplace_back(max(back->second, it->second), min(back->second, it->second));
            continue;
        }

        --front;
        if (back == s.end()) {
            // 最大值只有左侧候选，其前驱一定是它的全局最近值。
            pr.emplace_back(max(front->second, it->second), min(front->second, it->second));
            continue;
        }

        if (it->first - front->first > back->first - it->first) {
            // 右侧差值更小，当前元素只与后继形成好的配对。
            pr.emplace_back(max(back->second, it->second), min(back->second, it->second));
        } else if (it->first - front->first < back->first - it->first) {
            // 左侧差值更小，当前元素只与前驱形成好的配对。
            pr.emplace_back(max(front->second, it->second), min(front->second, it->second));
        } else {
            // 两侧差值相等，前驱和后继都是当前元素的全局最近值。
            pr.emplace_back(max(back->second, it->second), min(back->second, it->second));
            pr.emplace_back(max(front->second, it->second), min(front->second, it->second));
        }
    }
    // pair 默认先比较 right，再比较 left，正好得到离线扫描所需的顺序。
    sort(pr.begin(), pr.end());

    // qr 保存 ((r,l),原询问编号)，排序后按照 r、l、编号依次处理。
    vector<pair<pair<int, int>, int>> qr;
    qr.reserve(m);
    for (int i = 0; i < m; i++) {
        int l, r;
        cin >> l >> r;
        qr.emplace_back(pair(r - 1, l - 1), i);
    }
    sort(qr.begin(), qr.end());

    // idx 指向下一个尚未激活的好配对，end 是好配对总数。
    int idx = 0, end = pr.size();
    long long res = 0;
    // te 保存 (left,唯一编号)。唯一编号使端点相同的有序配对不会被 PBDS 去重。
    __gnu_pbds::tree<pair<int, int>, __gnu_pbds::null_type, less<>,
        __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> te;
    for (const auto &[fst, snd]: qr) {
        // fst 是离线排序键 (r,l)，snd 是询问在输入中的原编号。
        int r = fst.first, l = fst.second;
        // 激活所有较大端点 right 已经落在当前询问右端点之内的配对。
        while (idx < end && pr[idx].first <= r) {
            te.insert({pr[idx].second, idx++});
        }
        // te.size()-order_of_key({l,-1}) 统计 left>=l 的已激活配对。
        // 使用 snd+1 恢复题目要求的原询问编号权重。
        res += (te.size() - te.order_of_key({l, -1})) * (snd + 1);
    }
    cout << res << endl;
    return 0;
}
