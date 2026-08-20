#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class U {
    vector<int> vt;

public:
    U(int n) : vt(n + 1) {
        for (int i = 0; i <= n; ++i) vt[i] = i;
    }

    int find(int x) {
        if (x == vt[x]) {
            return x;
        }
        vt[x] = find(vt[x]);
        return vt[x];
    }

    void merge(int x, int y) {
        int rx = find(x), ry = find(y);
        vt[rx] = ry;
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> graph(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }

    U us(n);
    long long sum = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for (auto [v, w]: graph[1]) {
        pq.emplace(w, v);
    }
    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();
        if (us.find(1) == us.find(u)) {
            continue;
        }
        sum += w;
        us.merge(1, u);

        for (auto [v, vw]: graph[u]) {
            pq.emplace(vw, v);
        }
    }

    for (int i = 2; i <= n; ++i) {
        if (us.find(1) != us.find(i)) {
            cout << "orz" << endl;
            return 0;
        }
    }
    cout << sum << endl;
    return 0;
}
