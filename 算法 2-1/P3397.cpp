#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector v(n + 2, vector(n + 2, 0)), nums = v;
    for (int i = 0; i < m; i++) {
        int x1, x2, y1, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        v[x1][y1]++;
        v[x1][y2 + 1]--;
        v[x2 + 1][y1]--;
        v[x2 + 1][y2 + 1]++;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            nums[i][j] = nums[i - 1][j] + nums[i][j - 1] - nums[i - 1][j - 1] + v[i][j];
            cout << nums[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
