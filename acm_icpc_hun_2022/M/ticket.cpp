// Ez a kód még nincs kész. :(

#include <bits/stdc++.h>
using namespace std;

typedef vector<int> VLIST;
typedef vector<long long> VLLLIST;
typedef vector<vector<pair<int, int>>> ELIST;

void DFS_SUBTREE_SIZE(int parent, int curr, ELIST &edgelist,
                      VLIST &subtree_size) {
  int ss = 0;
  for (auto &n : edgelist[curr])
    if (parent != n.first) {
      int child = n.first;
      int weight = n.second;

      if (subtree_size[child] == -1) {
        DFS_SUBTREE_SIZE(curr, n.first, edgelist, subtree_size);
      }

      ss += subtree_size[child];
    }
  subtree_size[curr] = ss + 1;
}

void DFS_DISTCOST_0(int parent, int curr, ELIST &edgelist, VLIST &subtree_size,
                    VLLLIST &distcost0costs) {
  int dc = 0;
  for (auto &n : edgelist[curr])
    if (parent != n.first) {
      int child = n.first;
      int weight = n.second;

      if (distcost0costs[child] == -1) {
        DFS_DISTCOST_0(curr, n.first, edgelist, subtree_size, distcost0costs);
      }

      dc += distcost0costs[child] + weight * subtree_size[child];
    }
  distcost0costs[curr] = dc;
}

int main() {
  int n;
  cin >> n;
  vector<int> t;
  int tsum = 0;
  for (int i = 0; i < n; ++i) {
    int ti;
    cin >> ti;
    t.push_back(ti);
    tsum += ti;
  }
  ELIST edgelist(n);
  for (int i = 0; i < n - 1; ++i) {
    int ui, vi, wi;
    cin >> ui >> vi >> wi;
    --ui;
    --vi;
    edgelist[ui].push_back(make_pair(vi, wi));
    edgelist[vi].push_back(make_pair(ui, wi));
  }

  VLIST subtree_size(n, -1);
  DFS_SUBTREE_SIZE(-1, 0, edgelist, subtree_size);
  for (int i = 0; i < n; ++i) {
    cout << subtree_size[i] << " ";
  }
  cout << endl;

  VLLLIST distcost0costs(n, -1);
  DFS_DISTCOST_0(-1, 0, edgelist, subtree_size, distcost0costs);
  for (int i = 0; i < n; ++i) {
    cout << distcost0costs[i] << " ";
  }
  cout << endl;
  return 0;
}
