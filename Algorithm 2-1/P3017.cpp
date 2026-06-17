/**
 * Luogu P3017 - [USACO11MAR] Brownie Slicing G
 *
 * 问题描述：
 *   一块 r × c 的巧克力被分成 r × c 个格子，每格有一个甜蜜值。
 *   需要把整块巧克力切成 a × b 个非空小块（沿格子边界切），
 *   求所有切法中，"最甜的那一块"的甜蜜值之和最大是多少。
 *   即：最大化 min(每块的甜蜜值之和)。
 *
 * 解法：二分答案 + 二维前缀和 + 贪心切分
 *   1. 构建二维前缀和，使任意子矩形的元素和可在 O(1) 内求出
 *   2. 二分枚举最终答案 ans（即"最甜那一块的最小值"）：
 *      - check(ans) 判断是否存在一种切法，使每块的甜蜜值之和 ≥ ans
 *   3. check 中使用双层贪心切分：
 *      - 外层按"行"切：在每一行带内贪心地找出最小的 curRowCut，
 *        使 [lastRowCut+1, 1] ~ [curRowCut, c] 这段列方向条带的元素和 ≥ ans * b
 *        （即按列均分 b 列后，每列仍能保证 ≥ ans 的下界）
 *      - 内层在当前行带内按"列"切：贪心地累计列方向区间，
 *        使每个 a × b 子块的元素和 ≥ ans
 *      - 若能切满 a × b 个子块，则说明 ans 可行
 *
 * 复杂度分析：
 *   - 时间复杂度：O(log(maxNum) · r · c)
 *     其中 check 为 O(r·c)（贪心扫描线性）
 *   - 空间复杂度：O(r·c)
 */

#include <iostream>
#include <vector>
using namespace std;

// 全局变量：v 为二维前缀和数组（多开一圈便于处理边界）
vector<vector<int>> v;
// 巧克力行数 r，列数 c，切分目标 a 行 × b 列
int r, c, a, b;


/**
 * 利用二维前缀和求子矩形 (x1, y1) ~ (x2, y2) 的元素之和
 *
 * @param x1, y1  子矩阵左上角坐标（1-based）
 * @param x2, y2  子矩阵右下角坐标（1-based）
 * @return        子矩阵内所有格子的甜蜜值之和
 */
int area(int x1, int y1, int x2, int y2) {
    // 公式：右下 - 上 - 左 + 左上
    return v[x2][y2] - v[x1 - 1][y2] - v[x2][y1 - 1] + v[x1 - 1][y1 - 1];
}

/**
 * 检查答案 ans 是否可行：能否切出 a × b 个子块，使得每个子块的甜蜜值之和 ≥ ans
 *
 * 思路：
 *   - 先按"行带"划分（共 a 行），每个行带必须包含 b 列，元素和 ≥ ans * b
 *   - 在每个行带内，再按"列"切出 b 个 a 行 × ? 列的小块，每个小块元素和 ≥ ans
 *   - 任一行带或列无法凑够则说明 ans 不可行
 *
 * @param ans  当前二分的候选答案
 * @return     是否存在合法切法
 */
bool check(long long ans) {
    // 外层：枚举 a 个行带
    for (int i = 0, lastRowCut = 0, curRowCut = 1; i < a; i++, lastRowCut = curRowCut++) {
        // 先贪心推进 curRowCut，使当前行带 (lastRowCut+1..curRowCut, 1..c) 的总和 ≥ ans * b
        // 即要求平均到 b 列上每列 ≥ ans
        while (curRowCut <= r && area(lastRowCut + 1, 1, curRowCut, c) < ans * b) {
            curRowCut++;
        }

        // 在当前行带内，继续按列贪心切分 b 段
        while (curRowCut <= r) {
            int flag = true;  // 标记当前行带能否切满 b 段
            for (int j = 0, lastColCut = 0, curColCut = 1; j < b; j++, lastColCut = curColCut++) {
                // 贪心推进 curColCut，使子块 (lastRowCut+1, lastColCut+1) ~ (curRowCut, curColCut) ≥ ans
                while (curColCut <= c && area(lastRowCut + 1, lastColCut + 1, curRowCut, curColCut) < ans) {
                    curColCut++;
                }
                // 当前列方向不够切出一段 ≥ ans 的子块 → 该行带失败
                if (curColCut > c) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                // 当前行带成功切分 b 段，可继续处理下一个行带
                break;
            }
            // 否则说明当前行带宽度不足，需再扩一行（curRowCut 后移一位）后重试
            curRowCut++;
        }

        // 若行带结束时仍超出 r，说明 ans 不可行
        if (curRowCut > r) {
            return false;
        }
    }
    return true;
}

int main() {
    cin >> r >> c >> a >> b;  // r 行 c 列，切分为 a 行 × b 列 共 a*b 块
    // 二维前缀和数组（多开一圈，避免越界）
    v = vector(r + 2, vector(c + 2, 0));

    // 边读入边构建二维前缀和：
    //   v[i][j] = 原矩阵(1,1)~(i,j) 元素之和
    for (int i = 1; i <= r; i++) {
        for (int j = 1; j <= c; j++) {
            cin >> v[i][j];
            // 递推公式：当前格 = 自身 + 上方 + 左方 - 左上重复
            v[i][j] += v[i - 1][j] + v[i][j - 1] - v[i - 1][j - 1];
        }
    }

    // 二分搜索最大可行的 ans
    // sum: 整块巧克力的甜蜜值总和, maxNum: 单格最大可能甜蜜值（作为答案上界）
    long long sum = v[r][c], maxNum = sum / (a * b);
    long long L = 0, R = maxNum, ans = 0;
    while (L <= R) {
        long long mid = (L + R) / 2;
        if (check(mid)) {
            // mid 可行，尝试更大值
            L = mid + 1;
            ans = mid;
        } else {
            // mid 不可行，缩小上界
            R = mid - 1;
        }
    }
    cout << ans << endl;
    return 0;
}
