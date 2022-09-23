#include <bits/stdc++.h>

#ifdef LOCAL
#include "../algo/debug.h"
#else
#define debug(...) 42
#endif

using namespace std;

int snake_t(int i, int j) { return 2 * i + (i + j) % 2; }

int t(int i, int j, int hi, int hj, int m) {

  // Hook előtt
  if (i < hi) {
    return snake_t(i, j);
  }

  if (i == hi) {
    if (j == hj) {
      return snake_t(i, j);
    }
    if ((i+j)%2 == 0) {
        return snake_t(i, j);
    } else {
        return 2 * m - 1;
    }
  }

  // Hook után
  if (hj == j) {
    return snake_t(hi, hj) + (i - hi);
  } else {
    return snake_t(hi, hj) + 2 * ((m - 1) - hi) + 1 - (i - hi);
  }
}

int main() {

  int cases;
  cin >> cases;

  for (int i = 0; i < cases; ++i) {
    int m;
    cin >> m;

    vector<vector<int>> a(2);
    a[0].resize(m);
    a[1].resize(m);

    for (int j = 0; j < m; ++j) {
      cin >> a[0][j];
    }
    for (int j = 0; j < m; ++j) {
      cin >> a[1][j];
    }

    for (int hi = 0; hi < m; ++hi) {
      for (int hj = 0; hj < 2; ++hj) {
        debug(hi, hj);

        vector<vector<int>> h(2);
        h[0].resize(m);
        h[1].resize(m);

        for (int i = 0; i < m; ++i) {
          for (int j = 0; j < 2; ++j) {
            h[j][i] = a[j][i] - t(i, j, hi, hj, m) + 1;
          }
        }
        debug("Input:");
        debug(a);
        debug("Output:");
        debug(h);
        debug("--------");
      }
    }
  }

  return 0;
}