#include <bits/stdc++.h>

#ifdef LOCAL
#include "../algo/debug.h"
#else
#define debug(...) 42
#endif

using namespace std;

int main() {

  int k;
  cin>>k;

  for(int cases=0; cases < k; ++cases) {
  vector<int> a;
  vector<int> b;
  vector<int> dmins;
  vector<int> dmaxes;

    int n;
    cin >> n;
    for(int i=0; i<n; ++i) {
      int x;
      cin>>x;
      a.push_back(x);
    }

    for(int i=0; i<n; ++i) {
      int x;
      cin>>x;
      b.push_back(x);
    }

    for(int i=0; i<n; ++i) {
      int bmin = *lower_bound(b.begin(), b.end(), a[i]);
      dmins.push_back(bmin-a[i]);
    }

    int j = 0;
    for(int i=0; i<n; ++i) {
      j = max(i, j);
      for(; j<n-1; ++j) {
        if (a[j+1] > b[j]) {
          dmaxes.push_back(b[j]-a[i]);
          break;
        }
      }
      if(j==n-1) {
          dmaxes.push_back(b[j]-a[i]);
      }
    }

    debug(dmins);
    for(int i=0; i<n; ++i) {
      cout << dmins[i] << ((i == (n-1)) ? "" : " ");
    }
    cout<<endl;

    debug(dmaxes);
    for(int i=0; i<n; ++i) {
      cout << dmaxes[i] << ((i == (n-1)) ? "" : " ");
    }
    cout<<endl;
  }

  return 0;
}