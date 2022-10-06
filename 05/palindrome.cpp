#include <bits/stdc++.h>

#ifdef LOCAL
#include "../algo/debug.h"
#else
#define debug(...) 42
#endif

using namespace std;
typedef unsigned int goodnum;

vector<int> palindromes() {
  vector<int> p;
  p.push_back(0);
  for(int i=1; i<400; ++i) {
    auto str = to_string(i);
    auto str_rev = str;
    reverse(str_rev.begin(), str_rev.end());
    auto palindrome_str = str + str_rev.substr(1, str_rev.size() - 1);
    auto palindrome_str2 = str + str_rev;
    int palindrome = stoi(palindrome_str);
    int palindrome2 = stoi(palindrome_str2);
    if (palindrome <= 40000) {
      p.push_back(palindrome);
    }
    if (palindrome2 <= 40000) {
      p.push_back(palindrome2);
    }
  }
  sort(p.begin(), p.end());
  return p;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  goodnum MOD = 1000000007;

  auto p = palindromes();

  int max_sum = 40001;
  vector<vector<goodnum>> dp(2);

  for(int j=0; j<2; ++j) {
    dp[j].resize(max_sum);
  }

  dp[0][0] = 1;
  for (int sum = 1; sum < max_sum; ++sum) {
    dp[0][sum] = 0;
  }
  for (int i = 1; i < p.size(); ++i) {
    auto curr = i%2;
    auto prev = (i - 1) % 2;
    for (int sum = 0; sum < max_sum; ++sum) {
      dp[curr][sum] = dp[prev][sum];
      if(p[i] <= sum) {
        int diff = sum - p[i];
        dp[curr][sum] = (dp[prev][sum] + dp[curr][diff]) % MOD;
      }
    }
  }

  int tests; cin>>tests;
  int dprow = (p.size() - 1) % 2;
  for (int tc = 0; tc < tests; ++tc) {
    int sum; cin>>sum;
    cout << dp[dprow][sum] << endl;
  }
  return 0;
}
