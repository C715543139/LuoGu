/**
 * Luogu P2866 - [USACO06NOV] Bad Hair Day S
 *
 * 问题描述：
 *   第 i 头牛只能看见右侧连续一段比自己严格矮的牛，
 *   求所有奶牛可看见数量之和。
 *
 * 解法：单调递减栈
 *   1. 从左到右处理奶牛。栈从底到顶保存严格递减的高度，
 *      代表仍可能看见后续奶牛的左侧奶牛。
 *   2. 当前高度 num 到来时，弹出所有高度小于等于 num 的栈顶奶牛。
 *      它们无法看见当前奶牛，并且会被当前奶牛永久遮挡。
 *   3. 弹栈后，栈中每头奶牛都严格高于 num，且中间没有能遮挡它的奶牛，
 *      因此都能看见当前奶牛，将栈大小累加到答案。
 *   4. 将当前奶牛压栈，作为后续奶牛的潜在观察者。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n)，每头奶牛最多入栈、出栈各一次
 *   - 空间复杂度：O(n)
 */

#include <iostream>
#include <stack>
using namespace std;

int main() {
    int n;
    cin >> n;

    // 可见关系总数最大为 n(n-1)/2，使用 long long 保存
    long long count = 0;
    // 栈从底到顶严格递减，保存仍可能看见后续奶牛的高度
    stack<int> st;
    // 从左到右读入每头奶牛的高度
    for (int i = 1, num; i <= n; i++) {
        cin >> num;
        if (st.empty()) {
            // 第一头奶牛直接入栈
            st.push(num);
        } else {
            // 高度不大于当前奶牛的栈顶会被当前奶牛遮挡，后续无需保留
            while (!st.empty() && num >= st.top()) {
                st.pop();
            }
            // 剩余栈中奶牛均能看见当前奶牛，各贡献一组可见关系
            count += st.size();
            // 当前奶牛成为后续奶牛左侧的潜在观察者
            st.push(num);
        }
    }
    // 输出所有奶牛可见数量之和
    cout << count << endl;
    return 0;
}
