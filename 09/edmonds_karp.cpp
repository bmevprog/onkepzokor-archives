#include <bits/stdc++.h>
using namespace std;

// Edmonds-Karp algorithm.
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
    for (int i = 0; i < v; ++i) {
      this->residual[i].resize(v);
    }

    this->flow.resize(v);
    for (int i = 0; i < v; ++i) {
      this->flow[i].resize(v);
    }

    for (int i = 0; i < v; ++i) {
      for (int j = 0; j < v; ++j) {
        // Initial residual flow network is same as c.
        residual[i][j] = capacity[i][j];
        // Initial flow is 0.
        flow[i][j] = 0;
      }
    }
  }

  int BFS() {
    // Parents of vertices in BFS graph.
    parent = vector<int>(v, -1);
    // From source to vertex i the min capacity along BFS edges.
    vector<int> max_flow_possible(v, -1);

    for (int i = 0; i < v; ++i) {
      max_flow_possible[i] = 0;
    }

    // Source has infinite capacity.
    max_flow_possible[s] = INT_MAX;
    // -1 means parent not found, but source needs no parent.
    // If source has an incoming edge, this will make BFS ignore it.
    parent[s] = -2;

    queue<int> frontier;
    frontier.push(s);
    while (!frontier.empty()) {
      int curr = frontier.front();
      frontier.pop();
      for (int next = 0; next < v; ++next) {
        if (residual[curr][next] > 0 && parent[next] == -1) {
          parent[next] = curr;
          max_flow_possible[next] = min(residual[curr][next], max_flow_possible[curr]);
          if (next == t) {
            return max_flow_possible[t];
          } else {
            frontier.push(next);
          }
        }
      }
    }
    return 0;
  }

public:
  EdmondsKarp(vector<vector<int>> &capacity) {
    this->capacity = std::move(capacity);
    this->v = this->capacity.size();
  }

  int run(int s, int t) {
    init(s, t);

    while (int addflow = BFS()) {
      maxflow += addflow;
      int curr = t;
      while (curr != s) {
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
  // - If parent[i] == -1, then i is on the source's side (reachable from the
  // source).
  // - If parent[i] != -1 AND no j exists for which parent[j] = i, i is on the
  // edge of the cut.
  vector<int> cut() {
    vector<int> result;
    for (int i = 0; i < v; ++i) {
      if (parent[i] != -1) {
        result.push_back(i);
      }
    }
    return result;
  }
};

int main() {
  // Source, target, number of vertices.
  int s, t, v;
  cin >> s >> t >> v;

  // Directed capacity adjacency matrix.
  vector<vector<int>> capacity;
  capacity.resize(v);
  for (int i = 0; i < v; ++i) {
    capacity[i].resize(v);
    for (int j = 0; j < v; ++j) {
      cin >> capacity[i][j];
    }
  }

  EdmondsKarp flow(capacity);
  auto maxflow = flow.run(s, t);
  auto cut = flow.cut();

  cout << maxflow << endl;
  for (auto i : cut) {
    cout << (char)('A' + i) << ' ';
  }
  cout << endl;

  return 0;
}
