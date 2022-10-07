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

  if(a[0]%10 == 0) {
    cout << 1 << " ";
  } else {
    for (int j = 1; j < n; ++j) {
      if (s[j] % 10 == 0) {
        cout << j + 1 << " ";
        break;
      }
    }
  }
 
  for(int i=1; i<n; ++i) {
    bool voltMegoldas = false;
    for(int j=i+1; j<n; ++j) {
      if (s[i-1]%10 == s[j]%10) {
        cout << j - i + 1 << " ";
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
