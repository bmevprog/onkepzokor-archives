#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "../algo/debug.h"
#else
#define debug(...) 42
#endif

void fun(string &end_result) {
  int T;
  cin >> T;

  for (int k = 0; k < T; ++k) {
    int M;
    cin >> M;
    int move[2][M];
    int max[3] = {0}; // x y res
    for (int j = 0; j < 2; ++j) {
      for (int i = 0; i < M; ++i) {
        cin >> move[j][i];
        if (max[2] < move[j][i]) {
          max[0] = j;
          max[1] = i;
          max[2] = move[j][i];
        }
      }
    }
    if (max[0] == 0 && max[1] != 0) {
      if (max[1] % 2 != 1) {
        max[1] -= 1;
      }
    } else {
      if (max[1] % 2 != 0) {
        max[1] -= 1;
      }
    }
    int maxtime = 0;
    if (max[1] == 0) {
      for (int i = 1; i < M; ++i) {
        if (move[0][i] > maxtime) {
          maxtime = move[0][i] + 1;
        } else {
          maxtime++;
        }
      }
      for (int i = M - 1; i >= 0; --i) {
        if (move[1][i] > maxtime) {
          maxtime = move[1][i] + 1;
        } else {
          maxtime++;
        }
      }
    } else {
      if (max[0] == 1) {
        int rowinveert = 1;
        for (int i = 0; i < max[1]; ++i) {
          if (move[rowinveert][i] > maxtime) {
            maxtime = move[rowinveert][i] + 1;
          } else {
            maxtime++;
          }
          if ((rowinveert + i) % 2 == 0) {
            rowinveert = (rowinveert == 1) ? 0 : 1;
            i--;
          }
        }
        for (int i = max[1]; i < M; ++i) {
          if (move[0][i] > maxtime) {
            maxtime = move[0][i] + 1;
          } else {
            maxtime++;
          }
        }
        for (int i = M - 1; i >= max[1]; --i) {
          if (move[1][i] > maxtime) {
            maxtime = move[1][i] + 1;
          } else {
            maxtime++;
          }
        }
      } else {
        int rowinveert = 1;
        for (int i = 0; i < max[1]; ++i) {
          if (move[rowinveert][i] > maxtime) {
            maxtime = move[rowinveert][i] + 1;
          } else {
            maxtime++;
          }
          if ((rowinveert + i) % 2 == 0) {
            rowinveert = (rowinveert == 1) ? 0 : 1;
            i--;
          }
        }
        for (int i = max[1]; i < M; ++i) {
          if (move[1][i] > maxtime) {
            maxtime = move[1][i] + 1;
          } else {
            maxtime++;
          }
        }
        for (int i = M - 1; i >= max[1]; --i) {
          if (move[0][i] > maxtime) {
            maxtime = move[0][i] + 1;
          } else {
            maxtime++;
          }
        }
      }
    }
    end_result += to_string(maxtime) + '\n';
  }
}

#ifndef LOCAL
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  string a;
  fun(a);
  cout << a;
  return 0;
}
#endif
#ifdef LOCAL
void test(const string &a, string &b) {
  std::stringstream in(a);
  std::cin.rdbuf(in.rdbuf());
  fun(b);
}
#endif