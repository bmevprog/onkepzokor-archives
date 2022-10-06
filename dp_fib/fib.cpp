#include <bits/stdc++.h>

#ifdef LOCAL
#include "../algo/debug.h"
#else
#define debug(...) 42
#endif

using namespace std;

int f(int n) {
  if (n <= 2) {
    return 1;
  }
  return f(n - 1) + f(n - 2);
}

vector<int> mem_table(1000);

int f_memoize(int n) {
  if (n <= 2) {
    return 1;
  }

  if (mem_table[n] == 0) {
    mem_table[n] = f(n - 1) + f(n - 2);
  }

  return mem_table[n];
}

vector<int> dp_table(1000);

int f_dp(int n) {
  dp_table[1] = 1;
  dp_table[2] = 1;
  for(int i=3; i<=n; ++i) {
    dp_table[i] = dp_table[i-1] + dp_table[i-2];
  }
  return dp_table[n];
}

int main() {
  cout << f_dp(100) << endl;
  return 0;
}
