#include <bits/stdc++.h>
#include "../../algo/debug.h"

using namespace std;

int main() {
  int n; cin>>n;
  vector<int> in = vector<int>(n);
  vector<int> oneline = vector<int>(n);

  for(int i=0; i<n; ++i) {
    cin>>in[i];
  }

  set<int> pref = set<int>();
  for(int i=0; i<n; ++i) {
    pref.insert(i);
  }

  for(int i=0; i<n; ++i) {


    int dist = pref.find_by_order(in[i]);
    debug(i);
    debug(in[i]);
    debug(dist);
    
    int place = dist + in[i];

    debug(place);
    debug(pref);

    oneline[place] = i+1;

    pref.erase(place);
    debug(oneline);
  }

  debug(oneline);

  return 0;
}