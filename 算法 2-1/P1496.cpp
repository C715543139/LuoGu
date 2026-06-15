#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<pair<long long, long long>> v(n);
    for (int i = 0; i < n; i++) cin >> v[i].first >> v[i].second;
    sort(v.begin(), v.end());

    long long head = v[0].first, tail = v[0].second;
    long long sum = 0;
    for (int i = 1; i < n; i++) {
        long long curHead = v[i].first, curTail = v[i].second;
        if (curHead > tail) {
            sum += tail - head;
            head = curHead;
            tail = curTail;
        } else {
            tail = max(tail, curTail);
        }
    }
    sum += tail - head;
    cout << sum << endl;
    return 0;
}
