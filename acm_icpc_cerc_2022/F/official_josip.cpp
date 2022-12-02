#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> point;

const int mod = 1e9 + 7, p = 5009;

int sub(int x, int y) {
  x -= y;
  if(x < 0) return x + mod;
  return x;
}
int add(int x, int y) {
  x += y;
  if(x >= mod) return x - mod;
  return x;
}
int mul(int x, int y) {
  return (ll) x * y % mod;
}

char letter[4] = {'A', 'B', 'C', 'D'};

int n, m, K;

int main() {
  ios_base::sync_with_stdio(false); cin.tie(0);
  cin >> n >> m >> K;

  vector<int> pw(n);
  vector<vector<int>> f(m, vector<int>(4));
  vector<vector<int>> a(n, vector<int>(m));

  pw[0] = 1;
  for (int i = 1; i < n; ++i) {
    pw[i] = mul(p, pw[i - 1]);
  }

  for (int i = 0; i < n; ++i) {
    string s; cin >> s;
    for (int j = 0; j < m; ++j) for (int k = 0; k < 4; ++k) {
      if(s[j] != letter[k]) {
        f[j][k] = add(f[j][k], pw[i]);
      }
      else {
        a[i][j] = k;
      }
    }
  }

  int real = 0;
  for (int i = 0; i < n; ++i) real = add(real, mul(K, pw[i]));
  for (int i = 0; i < n; ++i) {
    real = sub(real, mul(K, pw[i]));

    int actual = 0;
    for (int j = 0; j < m; ++j) {
      actual = add(actual, f[j][a[i][j]]);
    }

    if(actual == real) {
      cout << i + 1;
      return 0;
    }

    real = add(real, mul(K, pw[i]));
  }
}
