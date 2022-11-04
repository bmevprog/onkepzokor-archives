#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector<int> DP(30,0);
string str;
vector<int> cnt(26,0), lens(26);

int main() {
    ios::sync_with_stdio(false);
    DP[1] = 1, DP[3] = 2;
    for (int len = 4; len < 30; ++len) DP[len] = DP[len-2] + DP[len-4];
    for (int i = 0, j = 0, k = 0; k < 26; ++k, ++j) {
        while (j == DP[i]) j = 0, ++i;
        lens[k] = i;
    }

    getline(cin, str);
    int ans = -3;
    for (char c : str) {
        if (c >= 'A' && c <= 'Z') c = c-'A'+'a';
        if (c >= 'a' && c <= 'z') ++cnt[c-'a'], ans += 3;
    }
    sort(cnt.rbegin(), cnt.rend());
    for (int k = 0; k < 26; ++k) {
        ans += lens[k]*cnt[k];
    }
    cout << ans << endl;

    return 0;
}
