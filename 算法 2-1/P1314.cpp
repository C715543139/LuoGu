#include <iostream>
#include <climits>
#include <vector>
using namespace std;

int main() {
    long long m, n, s;
    cin >> m >> n >> s;
    vector<int> w(m), v(m), l(n), r(n);
    for (int i = 0; i < m; i++) {
        cin >> w[i] >> v[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> l[i] >> r[i];
    }

    long long lW = 1, rW = 2000001, minSub = LONG_LONG_MAX;
    while (lW <= rW) {
        long long W = (lW + rW) / 2;

        // 构建前缀和
        vector sumW(m + 1, 0), sumV(m + 1, 0);
        for (int i = 1; i <= m; i++) {
            if (w[i - 1] >= W) {
                sumW[i] = sumW[i - 1] + 1;
                sumV[i] = sumV[i - 1] + v[i - 1];
            } else {
                sumW[i] = sumW[i - 1];
                sumV[i] = sumV[i - 1];
            }
        }

        // 计算 y
        long long y = 0;
        for (int i = 0; i < n; i++) {
            int left = l[i], right = r[i];
            y += (sumW[right] - sumW[left - 1]) * (sumV[right] - sumV[left - 1]);
        }
        minSub = min(minSub, abs(s - y));

        if (s > y) {
            rW = W - 1;
        } else {
            lW = W + 1;
        }
    }
    cout << minSub << endl;
    return 0;
}
