#include <bits/stdc++.h>

using namespace std;

set<int> unmerged;
map<int, int> age;
map<int, int> sizes;
map<pair<int,int>, int64_t> distances;
map<pair<int,int>, tuple<long double, int, int>> distances_normalized;

int main() {
  int n;
  cin>>n;
  vector<int64_t> x(n);
  vector<int64_t> y(n);

  for(int i=0; i<n; ++i) {
      cin>>x[i]>>y[i];
      unmerged.insert(i);
      age.insert({i, i});
      sizes.insert({i, 1});
  }

  int currage = n;

  for(int i=0; i<n; ++i) {
    for(int j=i+1; j<n; ++j) {
        int64_t dist = (x[i]-x[j])*(x[i]-x[j]) + (y[i]- y[j])*(y[i]- y[j]);
        distances.insert({{i, j}, dist});
        distances_normalized.insert({{i, j}, {dist, min(i,j), max(i,j)}});

    }
  }

  while(!distances.empty()){
    auto it = min_element(
      distances_normalized.begin(),
      distances_normalized.end(),
       [](const pair<pair<int,int>, tuple<long double, int, int>>& x, const pair<pair<int,int>, tuple<long double, int, int>>& y) { 
            if(get<0>(x.second) < get<0>(y.second)) return true;
            if(get<0>(x.second) > get<0>(y.second)) return false;

            if(get<1>(x.second) < get<1>(y.second)) return true;
            if(get<1>(x.second) > get<1>(y.second)) return false;

            if(get<2>(x.second) < get<2>(y.second)) return true;
            if(get<2>(x.second) > get<2>(y.second)) return false;
            
        });
      
    auto a = it->first.first;
    auto b = it->first.second;

    distances.erase(distances.find({min(a,b),max(a,b)}));
    distances_normalized.erase(distances_normalized.find({min(a,b),max(a,b)}));

    auto newindex = min(a,b);

    auto asize = sizes.find(a);
    auto bsize = sizes.find(b);
    auto newsize = asize->second+bsize->second;

    cout << newsize << endl;

    sizes.erase(asize);
    sizes.erase(bsize);
    sizes.insert({newindex, newsize});

    auto aage = age.find(a);
    auto bage = age.find(b);

    age.erase(aage);
    age.erase(bage);
    age.insert({newindex, currage});

    unmerged.erase(unmerged.find(a));
    unmerged.erase(unmerged.find(b));

    for(auto u : unmerged) {

      auto ita = distances.find({min(a,u),max(a,u)});
      auto itb = distances.find({min(b,u),max(b,u)});
      auto itanorm = distances_normalized.find({min(a,u),max(a,u)});
      auto itbnorm = distances_normalized.find({min(b,u),max(b,u)});

      distances.erase(ita);
      distances.erase(itb);
      distances_normalized.erase(itanorm);
      distances_normalized.erase(itbnorm);

      auto uage = age.find(u)->second;
      auto usize = sizes.find(u)->second;

      auto dista = ita->second;
      auto distb = itb->second;
      auto newdist = dista + distb;
      auto newdistnorm = (((long double) newdist) / usize) / newsize;

      distances.insert({{min(newindex, u), max(newindex,u)}, newdist});
      distances_normalized.insert({{min(newindex, u), max(newindex,u)}, {newdistnorm, min(uage, currage), max(uage, currage)}});
    }

    unmerged.insert(newindex);
    ++currage;
  }

  return 0;
}