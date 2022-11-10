#include <bits/stdc++.h>

#ifdef LOCAL
#include "../algo/debug.h"
#else
#define debug(...) 42
#endif

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

    this->residual = vector<vector<int>>(v, vector<int>(v));
    this->flow = vector<vector<int>>(v, vector<int>(v));

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
      debug(curr);
      frontier.pop();
      for (int next = 0; next < v; ++next) {
        if (residual[curr][next] > 0 && parent[next] == -1) {
          parent[next] = curr;
          debug(curr, next);
          max_flow_possible[next] = min(residual[curr][next], max_flow_possible[curr]);
          if (next == t) {
            return max_flow_possible[t];
          } else {
            frontier.push(next);
          }
        }
      }
    }
    debug("BFS END", parent);
    return 0;
  }

public:
  EdmondsKarp(const vector<vector<int>> &capacity) {
    this->capacity = capacity;
    this->v = this->capacity.size();
  }

  int run(int s, int t) {
    init(s, t);

    while (int addflow = BFS()) {
      debug("AFTER BFS");
      maxflow += addflow;
      debug(parent);
      int curr = t;
      while (curr != s) {
        int pcurr = parent[curr];
        residual[pcurr][curr] = residual[pcurr][curr] - addflow;
        residual[curr][pcurr] = residual[curr][pcurr] + addflow;
        flow[pcurr][curr] = flow[pcurr][curr] + addflow;
        curr = pcurr;
        debug(curr);
      }
      debug(flow);
      debug(residual);
    }
    debug("DONE FLOW");
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

void solve() {

  // num of ice pieces
  // 1 <= ... <= 100
  int floes; 
  int penguins = 0;

  // max jump distance
  // 0 <= ... <= 100,000
  double maxd, maxdsq;

  cin >> floes >> maxd; maxdsq = maxd * maxd;

  vector<int> x(floes), y(floes), n(floes), m(floes);

  for(int i=0; i<floes; ++i) {
    // ith ice floe coordinates 
    // -10,000 <= ... <= 10,000
    int xi, yi;
    
    // number of penguins on it
    // 0 <= ... <= 10
    int ni;

    // max num of penguins that can go through (jump off)
    // 1 <= ... <= 200
    int mi;

    cin >> xi >> yi >> ni >> mi;
    x[i] = xi; y[i] = yi; n[i] = ni; m[i] = mi;

    penguins += ni;
  }

  debug(x);
  debug(y);
  debug(n);
  debug(m);

  // VERTICES:
  // i in 0..(floes-1):
  //   EDGE: 2*i -> 2*i+1 = mi: max num of penguins that can go through the ith floe
  // i in 0..(floes-1) and j in 0..(floes-1):
  //   if we can jump between i and j:
  //     EDGE: 2*i+1 -> 2*j = INT_MAX
  //     EDGE: 2*j+1 -> 2*i = INT_MAX
  // + artificial source: 2*floes, 2*floes -> 2*i = ni: number of penguins on the ith floe
  //
  // TARGETS:
  //  i in 0..(floes-1):
  //    target = 2*i: we don't jump off this floe, don't go through its capacity
  
  // Directed capacity adjacency matrix.
  int v = 2*floes + 1;
  vector<vector<int>> capacity(v, vector<int>(v));
  int s = 2*floes;

  for(int i=0; i<floes; ++i) {
    capacity[2*i][2*i+1] = m[i];
    capacity[s][2*i] = n[i];

    for(int j=0; j<floes; ++j) if (i!=j) {
      auto distsq = (x[i]-x[j]) * (x[i]-x[j]) + (y[i]-y[j]) * (y[i]-y[j]);

      if(distsq <= maxdsq) { // question: allow epsilon diff here?
        capacity[2*i+1][2*j] = INT_MAX;
        capacity[2*j+1][2*i] = INT_MAX;
      }
    }
  }

  debug(capacity);
  EdmondsKarp flow(capacity);

  bool found = false;
  for(int i=0; i<floes; ++i) {
    int t = 2*i;
    debug("BEGIN TO TARGET:", t);
    // s,t,v = source, target, num of vertices

    auto maxflow = flow.run(s, t);
    debug(maxflow);
    auto cut = flow.cut();
    debug(cut);

    if (maxflow == penguins) {
      cout << (found ? " ": "") << i;
      found = true;
    }
  }

  if(!found) {
      cout << -1;
    }

  cout << endl;
}

int main() {
  int cases; cin>>cases;

  while(cases--) {
    solve();
  }

  return 0;
}