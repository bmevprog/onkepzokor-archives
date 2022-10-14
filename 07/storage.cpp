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

  int n,store_size; cin>>n>>store_size;

  vector<int> weights(n);
  for(int i=0; i<n; ++i) {
    cin>>weights[i];
  }

  // DP[i][j][W] = Az elso i db suly felhasznalasaval
  // j meretu subsetbol aminek pont W a merete ennyi db van.
  //
  // Tabla merete: N * N * K
  // DP[0][0][0] = 1
  // Altalanosan: Az i. elem vagy 1. nincs benne vagy 2. benne van a subsetekben:
  // DP[i][j][W] = DP[i-1][j][W] + DP[i-1][j-1][W-weights[i]]
  // Ha W-weights[i] < 0 vagy j-1 < 0 akkor a masodik tag nincs.
  // Atalakitva W < weights[i] vagy j == 0.
  //
  // Az egyik elemet mindig kihagyjuk a bulibol es a tobbire kiszamoljuk ezt a
  // segedtablat, majd a tablaban bizonyos cellak osszege lesz a megoldas.
  //
  // Tfh. az N. elem van kihagyva, erre a megoldas:
  // Minden j=0..N-2-re sum([DP[N-1][j][wcurr] for wcurr in W-w_N+1 ... W])
  //
  // Ha nem az N. elem van kihagyva hanem valaki mas akkor swappeljuk az N.
  // helyre es csinaljuk ugyanezt (skipped valtozo).
  //
  // O(N^3*K)

  for(int skipped=0; skipped<n; ++skipped) {
    swap(weights[n-1], weights[skipped]);
    debug("Most ez a súlyok sorrendje:");
    debug(weights);
    vector<vector<vector<int>>> dp(n+1);
    for(int i=0; i<n; ++i) {
      dp[i].resize(n);
      for(int j=0; j<n; ++j){
        dp[i][j].resize(store_size+1);
      }
    }

    dp[0][0][0] = 1; // Az elso 0 elem felhasznalasaval 0 meretu subsetbol ami 0 sulyu 1 db van.

    for(int i=1; i <= n-1; ++i) { // Hanyadik item (1-tol indexelunk, utolso elemet kihagyjuk a bulibol).
      for (int j=0; j < n; ++j) { // Subsetben levo elemek darabszama, 0 meretut for cikluson kivul letudtuk.
        for(int k=0; k<=store_size; ++k) { // Subsetben levo elemek osszsulya.
          dp[i][j][k] = dp[i-1][j][k];
          if (weights[i - 1] <= k && 0 < j) { // 1-tol indexeltunk vvvvvvvv
            dp[i][j][k] += dp[i - 1][j - 1][k - weights[i - 1]];
          }
        }
      }
      debug("Ennyiedik elem és ezelőttieket nézzük:");
      debug(i);
      debug("Ez a tábla:");
      debug(dp[i]);
    }

    for(int j=1; j<=n-1; ++j)
    {
      int sum = 0;
      for(int wcurr=store_size-weights[n-1]+1; wcurr <= store_size; ++wcurr) {
        sum += dp[n-1][j][wcurr];
      }
      cout<<sum<<" ";
    }
    cout << endl;

    // Don't forget to swap back before going forward!
    swap(weights[n - 1], weights[skipped]);
  }

  return 0;
}

