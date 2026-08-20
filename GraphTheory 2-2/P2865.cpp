#include <iostream>
#include <queue>
#include <vector>
#include <climits>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, R;
    cin >> N >> R;
    vector<vector<pair<int, int>>> graph(N + 1);
    for (int i = 0; i < R; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }

    vector minDist(N + 1, INT_MAX), secDist(N + 1, INT_MAX);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    minDist[1] = 0;
    pq.emplace(0, 1);
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > secDist[u]) {
            continue;
        }

        for (auto [v, w]: graph[u]) {
            if (minDist[v] > d + w) {
                swap(minDist[v], secDist[v]);
                minDist[v] = d + w;
                pq.emplace(minDist[v], v);
            } else if (minDist[v] < d + w && d + w < secDist[v]) {
                secDist[v] = d + w;
                pq.emplace(secDist[v], v);
            }
        }
    }
    cout << secDist[N] << endl;
    return 0;
}
