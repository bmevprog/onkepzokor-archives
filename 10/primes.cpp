#include <bits/stdc++.h>
using namespace std;

int main() {
  int n = 32000;
  vector<bool> primes(n, true);

  for(int i=2; i<n; ++i) {
    if(primes[i]) {
      cout << i << ", ";
      int tobbszoros = i;
      while(tobbszoros*i < n) {
        primes[tobbszoros*i] = false;
        ++tobbszoros;
      }
    }
  }

  return 0;
}