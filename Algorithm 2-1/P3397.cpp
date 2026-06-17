/**
 * Luogu P3397 - 地毯
 *
 * 问题描述：
 *   在 n × n 的网格上铺 m 块地毯，每块地毯覆盖一个子矩形区域
 *   (x1, y1) ~ (x2, y2)，求最终每个格子被多少块地毯覆盖。
 *
 * 解法：二维差分 + 二维前缀和
 *   1. 利用二维差分记录每块地毯的覆盖范围：
 *        v[x1][y1]         += 1
 *        v[x1][y2 + 1]     -= 1
 *        v[x2 + 1][y1]     -= 1
 *        v[x2 + 1][y2 + 1] += 1
 *   2. 通过二维前缀和还原每个格子的最终覆盖次数：
 *        nums[i][j] = nums[i-1][j] + nums[i][j-1] - nums[i-1][j-1] + v[i][j]
 *   3. 按行输出 nums[i][j]
 *
 * 复杂度分析：
 *   - 时间复杂度：O(n^2 + m)，m 次操作每次 O(1)
 *   - 空间复杂度：O(n^2)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;  // n: 网格大小, m: 地毯数量
    // v: 二维差分数组（多开一圈避免越界）
    // nums: 用于还原的二维前缀和数组
    vector v(n + 2, vector(n + 2, 0)), nums = v;
    // 处理 m 块地毯，每块进行二维差分
    for (int i = 0; i < m; i++) {
        int x1, x2, y1, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        // 二维差分的"四顶点修改"：
        // 左上角 +1，右上角 -1，左下角 -1，右下角 +1
        v[x1][y1]++;
        v[x1][y2 + 1]--;
        v[x2 + 1][y1]--;
        v[x2 + 1][y2 + 1]++;
    }
    // 通过二维前缀和还原每个格子的覆盖次数，并按行输出
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            // 递推公式：当前前缀和 = 上一行 + 上一列 - 左上重复 + 差分值
            nums[i][j] = nums[i - 1][j] + nums[i][j - 1] - nums[i - 1][j - 1] + v[i][j];
            cout << nums[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
