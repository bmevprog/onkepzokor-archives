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
    if ((i + j) % 2 == 0) {
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

int t_max(int hi, int hj, int m) {
  // versions that can be maximal
  vector<int> versions;
  versions.push_back(t(hi, hj, hi, hj, m));
  versions.push_back(t(hi, (hj + 1) % 2, hi, hj, m));
  versions.push_back(t(m - 1, hj, hi, hj, m));
  versions.push_back(t(0, (hj + 1) % 2, hi, hj, m));
  versions.push_back(0);

  return *max_element(versions.begin(), versions.end());
}

int t_h_max(int i, int j, int m) {
  // versions that can be maximal
  vector<int> versions;
  versions.push_back(0);
  versions.push_back(t(i, j, m - 1, j, m));
  versions.push_back(t(i, j, m - 1, (j + 1) % 2, m));

  if (0 <= i - 1) {
    versions.push_back(t(i, j, i - 1, j, m));
    versions.push_back(t(i, j, i - 1, (j + 1) % 2, m));
  }
  return *max_element(versions.begin(), versions.end());
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

    int min = -1;
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < 2; ++j) {
            auto res = t_h_max(i, j, m);
            auto totalres = max(a[j][i] - res + 1, 0);
          }
        }

        int time = maxval + 2 * m - 1;

        if (min == -1 || min > time) {
          min = time;
        }

    cout << min << endl;
  }

  return 0;
}