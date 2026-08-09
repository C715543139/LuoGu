/**
 * Luogu P1226 - 【模板】快速幂
 *
 * 问题描述：
 *   计算 a^b mod p，并按题目要求的格式输出结果。
 *
 * 解法：递归二分幂
 *   设 res = a^(b / 2) mod p。
 *   当 b 为偶数时，a^b = (a^(b / 2))^2；
 *   当 b 为奇数时，再额外乘一个 a。
 *   每次递归将指数减半，因此只需 O(log b) 次乘法。
 *
 * 复杂度分析：
 *   - 时间复杂度：O(log b)
 *   - 递归栈空间复杂度：O(log b)
 */

#include <iostream>
using namespace std;

// 返回 a^n mod p。unsigned long long 用于保存乘法中间结果，避免接近 2^31 的数相乘溢出。
unsigned long long fastExp(int a, int n, int p) {
    // a^0 = 1；题目保证 p >= 2，因此 1 已经是合法余数。
    if (n == 0) return 1;

    // 递归计算 a^(floor(n / 2)) mod p，并在后续分支中复用该结果。
    unsigned long long res = fastExp(a, n / 2, p);
    if (n % 2 == 0) {
        // n 为偶数：a^n = (a^(n / 2))^2。
        return res * res % p;
    }

    // n 为奇数：a^n = (a^(floor(n / 2)))^2 * a。
    return res * res % p * a % p;
}

int main() {
    // n 对应题目中的指数 b。
    int a, n, p;
    cin >> a >> n >> p;

    // 按题目规定输出“a^b mod p=结果”。
    cout << a << '^' << n << " mod " << p << '=' << fastExp(a, n, p) << endl;
    return 0;
}
