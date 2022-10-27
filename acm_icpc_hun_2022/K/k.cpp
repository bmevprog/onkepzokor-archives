#include <bits/stdc++.h>

using namespace std;

struct MaxMatchBipartiteGraph {
	vector<vector<int>> adj;
	vector<bool> visited;
	vector<int> match;

	MaxMatchBipartiteGraph(int n) {
    adj.resize(n);
    match.resize(n, -1);
    visited.resize(n);
	}

	//Augmenting path.
	bool DFS(int from) {
		if(visited[from]) return false;
		visited[from] = true;
		for(auto& u: adj[from]) {
			if(match[u] == -1 || DFS(match[u])) {
					match[from] = u;
					match[u] = from;
					return true;
			}
		}
		return false;
	}

	void solve () {
		for(int i=0; i<adj.size(); ++i) if(match[i] == -1) {
      visited.clear();
      visited.resize(adj.size());
			DFS(i);
		}

	}

};


int main() {
  int n;
  cin>>n;

  vector<string> chars(n);
  MaxMatchBipartiteGraph m(n);
  for(int i=0; i<n; ++i) {
    cin>>chars[i];
  }

  for(int i=0; i<n; ++i) {
    for(int j=0; j<n; ++j) {
      int diff=0;
      for(int k=0; k<chars[i].size(); ++k) {
        if(chars[i][k] != chars[j][k]) ++diff;
      }
      if (diff == 2) {
        m.adj[i].push_back(j);
        m.adj[j].push_back(i);
      }
    }
  }

  m.solve();

  int size = 0;
  for(int i=0; i<n; ++i) {
    if(m.match[i] != -1) {
      ++size;
    }
  }

  // Max ftln
  cout << n - size / 2 <<endl;

	return 0;
}
