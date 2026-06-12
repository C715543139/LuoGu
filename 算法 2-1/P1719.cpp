#include <iostream>
#include <climits>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector v(n + 1, vector(n + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> v[i][j];
            v[i][j] += v[i - 1][j] + v[i][j - 1] - v[i - 1][j - 1];
        }
    }

    int maxArea = INT_MIN;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            for (int k = i; k <= n; k++) {
                for (int l = j; l <= n; l++) {
                    maxArea = max(maxArea, v[k][l] + v[i - 1][j - 1] - v[k][j - 1] - v[i - 1][l]);
                }
            }
        }
    }
    cout << maxArea << endl;
    return 0;
}