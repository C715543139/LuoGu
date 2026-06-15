#include <iostream>
#include <vector>
using namespace std;

void solve() {
    int n, p;
    cin >> n >> p;
    vector v(n, 0);

    int add = 0;
    for (int i = 0; i < n; i++) cin >> v[i];
    for (int i = 0; i < p; i++) {
        int x, y, z;
        cin >> x >> y >> z;
        if (x == 1 && y == n) {
            add += z;
        } else if (y - x > n / 2) {
            add += z;
            for (int j = 0; j < x - 1; j++) v[j] -= z;
            for (int j = y; j < n; j++) v[j] -= z;
        } else {
            for (int j = x - 1; j < y; j++) v[j] += z;
        }
    }

    int res = 2147483647;
    for (int num: v) res = min(res, num);
    cout << res + add << endl;
}

void solve2() {
    int n, p;
    cin >> n >> p;
    vector v(n, 0);

    cin >> v[0];
    for (int i = 1, last = v[0], cur; i < n; i++) { // 差合，前缀和的逆原理
        cin >> cur;
        v[i] = cur - last;
        last = cur;
    }
    for (int i = 0; i < p; i++) {
        int x, y, z;
        cin >> x >> y >> z;

        v[x - 1] += z;
        if (y < n) v[y] -= z;
    }

    int num = v[0], res = num;
    for (int i = 1; i < n; i++) {
        num += v[i];
        res = min(res, num);
    }
    cout << res << endl;
}

int main() {
    solve2();
    return 0;
}
