#include <bits/stdc++.h>
using i64 = long long;
using namespace std;

int a[2][200005];
int f[2][200005];

int g(int x) {
  if (x < 0)
    return 0;
  else
    return x;
}
void solve() {
  int n;
  cin >> n;
  for (int i = 0; i < 2; ++i) {
    for (int j = 1; j <= n; ++j) {
      cin >> a[i][j];
    }
  }
  a[0][1] = -1;
  f[0][n] = a[0][n];
  f[1][n] = a[1][n];
  for (int i = 0; i < 2; ++i) {
    for (int j = n - 1; j >= 1; --j) {
      f[i][j] = max(f[i][j + 1] - 1, a[i][j]);
    }
  }
  f[0][n] = max(a[1][n] - 1, a[0][n]);
  f[1][n] = max(a[0][n] - 1, a[1][n]);
  for (int i = 0; i < 2; ++i) {
    for (int j = n - 1; j >= 1; --j) {
      f[i][j] =
          max(a[i][j], max(f[i][j + 1] - 1, a[i ^ 1][j] - 2 * (n - j) - 1));
    }
  }
  int ans = f[0][1] + 2 * n, rec = a[1][1] + 1;
  for (int i = 2, p = 1; i <= n; ++i, p ^= 1) {
    ans = min(ans, rec + g(f[p][i] - rec) + 2 * (n - i + 1));
    rec = max(rec + 1, a[p][i] + 1);
    rec = max(rec + 1, a[p ^ 1][i] + 1);
  }
  ans = min(ans, rec);
  cout << ans << "\n";
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}