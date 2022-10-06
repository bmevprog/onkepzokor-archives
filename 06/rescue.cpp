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
  vector<int> a(n);
  for(int i=0; i<n; ++i) {
    cin>>a[i];
  }
  vector<int> s(n);
  s[0] = a[0];
  for(int i=1; i<n; ++i) {
    s[i] = s[i-1] + a[i];
  }

  debug(a);
  debug(s);

  return 0;
}
