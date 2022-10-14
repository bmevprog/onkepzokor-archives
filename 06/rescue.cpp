#include <bits/stdc++.h>

#ifdef LOCAL
#include "../algo/debug.h"
#else
#define debug(...) 42
#endif

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n; cin>>n;
  vector<int> a(n+1);
  for(int i=1; i<=n; ++i) {
    cin>>a[i];
  }
  vector<int> s(n);
  for(int i=1; i<=n; ++i) {
    s[i] = s[i-1] + a[i];
  }

  for(int i=0; i<n; ++i) {
    bool voltMegoldas = false;
    for(int j=i+1; j<=n; ++j) {
      if (s[i]%10 == s[j]%10) {
        cout << j - i << " ";
        voltMegoldas = true;
        break;
      }
    }
    if (!voltMegoldas) {
      cout << -1 << " ";
    }
  }

  return 0;
}
