#include <bits/stdc++.h>
using namespace std;


//Edmonds-Karp algorithm.
class EdmondsKarp {

  // Source, target.
  int s, t;

  // Original network: directed capacity adjacency matrix.
  vector<vector<int>> capacity;

  // Vertex count.
  int v;

  // Residual flow network.
  vector<vector<int>> residual;

  // Current flow.
  vector<vector<int>> flow;

  // Max flow found.
  int maxflow;

  // Parent table in BFS.
  vector<int> parent;

  void init(int s, int t) {
    this->maxflow = 0;
    this->s = s;
    this->t = t;

    this->residual.resize(v);
    for(int i=0; i<v; ++i) {
      this->residual[i].resize(v);
    }

    this->flow.resize(v);
    for(int i=0; i<v; ++i) {
      this->flow[i].resize(v);
    }

    for(int i=0; i<v; ++i)
    for(int j=0; j<v; ++j) {
      residual[i][j] = capacity[i][j]; // Initial residual flow network is same as c.
      flow[i][j] = 0; // Initial flow is 0.
    }
  }

  

  int BFS() {
    parent = vector<int>(v, -1);
    vector<int> min_capacity(v, -1);

    for(int i=0; i<v; ++i) {
      parent[i] = -1;
      min_capacity[i] = 0;
    }

    min_capacity[s] = INT_MAX;
    // parent[s] = -2;

    queue<int> frontier;
    frontier.push(s);
    while(!frontier.empty()) {
      int curr = frontier.front(); frontier.pop();
      for(int i=0; i<v; ++i) if(residual[curr][i] > 0 && parent[i] == -1) {
        parent[i] = curr;
        min_capacity[i] = min(residual[curr][i], min_capacity[curr]);
        if(i == t) {
          return min_capacity[t];
        } else {
          frontier.push(i);	
        }
      }
    }
    return 0;
  }

public:

  EdmondsKarp(vector<vector<int>>& capacity) {
    this->capacity = std::move(capacity);
    this->v = this->capacity.size();
  }

  int run(int s, int t) {
    init(s,t);

    while(int addflow = BFS()) {
      maxflow += addflow;
      int curr = t;
      while(curr != s) {
        int pcurr = parent[curr];
        residual[pcurr][curr] = residual[pcurr][curr] - addflow;
        residual[curr][pcurr] = residual[curr][pcurr] + addflow;
        flow[pcurr][curr] = flow[pcurr][curr] + addflow;
        curr = pcurr;
      }
    }

    return maxflow;
  }

  // Call after run.
  // After last BFS:
  // - If parent[i] == -1, then i is on the source's side (reachable from the source).
  // - If parent[i] != -1 AND no j exists for which parent[j] = i, i is on the edge of the cut.
  vector<int> cut() {
    vector<int> result;
    for(int i=0; i<v; ++i) {
      if(parent[i]!=-1) {
        result.push_back(i);
      }
    }
    return result;
  }
};

int main () {
  // Source, target, number of vertices.
  int s,t,v;
  cin>>s>>t>>v;

  // Directed capacity adjacency matrix.
  vector<vector<int>> capacity;
  capacity.resize(v);
  for(int i=0; i<v; ++i) {
    capacity[i].resize(v);
    for(int j=0; j<v; ++j) {
      cin>>capacity[i][j];
    }
  }

  EdmondsKarp flow(c);
  auto maxflow = flow.run(s,t);
  auto cut = flow.cut();

  cout<<maxflow<<endl;
  for(auto i: cut) {
    cout<<(char)('A'+i)<<' ';
  }
  cout<<endl;

  return 0;
}
