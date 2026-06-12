#include <iostream>
#include <vector>
using namespace std;

int main() {
    int m;
    cin >> m;
    vector v(m + 1, 0);
    for (int i = 1, temp; i <= m; i++) {
        cin >> temp;
        v[i] = temp + v[i - 1];
    }

    int n;
    cin >> n;
    for (int i = 0, p, q; i < n; i++) {
        cin >> p >> q;
        cout << v[q] - v[p - 1] << endl;
    }
    return 0;
}