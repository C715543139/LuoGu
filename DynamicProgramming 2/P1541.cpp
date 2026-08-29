/**
 * Luogu P1541 - [NOIP 2010 提高组] 乌龟棋
 *
 * 问题描述：
 *   乌龟从棋盘第 1 格出发，使用给定的 1、2、3、4 四种爬行卡片到达终点。
 *   每张卡片只能使用一次，乌龟每到达一个格子就获得该格子的分数，求最大得分。
 *
 * 解法：四维动态规划
 *   由于同一种类型的卡片效果完全相同，因此不需要区分具体卡片，
 *   只需要记录四种卡片分别已经使用了多少张。
 *
 *   设 dp[i][j][k][l] 表示使用了 i 张 1 步卡片、j 张 2 步卡片、
 *   k 张 3 步卡片和 l 张 4 步卡片时，能够获得的最大分数。
 *
 *   使用这些卡片后，乌龟所在的位置唯一确定为：
 *     pos = 1 + i + 2 * j + 3 * k + 4 * l
 *
 *   状态转移时，枚举最后使用的卡片类型：
 *   - 最后使用 1 步卡片：从 dp[i-1][j][k][l] 转移；
 *   - 最后使用 2 步卡片：从 dp[i][j-1][k][l] 转移；
 *   - 最后使用 3 步卡片：从 dp[i][j][k-1][l] 转移；
 *   - 最后使用 4 步卡片：从 dp[i][j][k][l-1] 转移。
 *
 *   每次转移都加上当前到达位置 pos 的分数。
 *   初始时乌龟位于第 1 格，因此 dp[0][0][0][0] 初始化为 points[1]。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(c1 * c2 * c3 * c4)，其中 ci 为第 i 类卡片的数量；
 *   - 空间复杂度：O(c1 * c2 * c3 * c4)。
 */

#include <iostream>
#include <vector>
using namespace std;


int main() {
    // 读入棋盘格子数和爬行卡片总数
    int n, m;
    cin >> n >> m;

    // points[i] 表示棋盘第 i 格的分数，棋盘下标从 1 开始
    vector points(n + 1, 0);

    // cards[i] 表示数字为 i 的爬行卡片数量
    vector<int> cards(5);

    // 读入每个棋盘格子的分数
    for (int i = 1; i <= n; i++) {
        cin >> points[i];
    }

    // 统计四种爬行卡片的数量
    for (int i = 0; i < m; i++) {
        int card;
        cin >> card;
        cards[card]++;
    }

    // dp[i][j][k][l] 表示四种卡片分别使用 i、j、k、l 张时的最大得分
    vector dp(41, vector(41, vector(41, vector(41, 0))));

    // 乌龟从第 1 格出发，自动获得起点分数
    dp[0][0][0][0] = points[1];

    // 枚举四种卡片的使用数量
    for (int i = 0; i <= cards[1]; i++) {
        for (int j = 0; j <= cards[2]; j++) {
            for (int k = 0; k <= cards[3]; k++) {
                for (int l = 0; l <= cards[4]; l++) {
                    // 使用这些卡片后，乌龟所在的位置由使用数量唯一确定
                    int &cur = dp[i][j][k][l];
                    int pos = 1 + i + j * 2 + k * 3 + l * 4;

                    // 最后使用一张 1 步卡片
                    if (i != 0) {
                        cur = max(cur, dp[i - 1][j][k][l] + points[pos]);
                    }

                    // 最后使用一张 2 步卡片
                    if (j != 0) {
                        cur = max(cur, dp[i][j - 1][k][l] + points[pos]);
                    }

                    // 最后使用一张 3 步卡片
                    if (k != 0) {
                        cur = max(cur, dp[i][j][k - 1][l] + points[pos]);
                    }

                    // 最后使用一张 4 步卡片
                    if (l != 0) {
                        cur = max(cur, dp[i][j][k][l - 1] + points[pos]);
                    }
                }
            }
        }
    }

    // 所有卡片都使用完时，输出能够获得的最大得分
    cout << dp[cards[1]][cards[2]][cards[3]][cards[4]] << endl;
    return 0;
}
