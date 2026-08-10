#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector v(n, 0), x(n, 0);
    for (int i = 0; i < n; i++) cin >> v[i] >> x[i];

    long long sum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            sum += max(v[i], v[j]) * abs(x[i] - x[j]);
        }
    }
    cout << sum << endl;
    return 0;
}
