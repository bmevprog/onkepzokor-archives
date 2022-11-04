#include <iostream>
#include <vector>
#include <string>

using namespace std;

int n, p,q;
vector<string> words;
vector<int> odd, even;
vector<vector<int>> e;
vector<int> mt;
vector<bool> used;

int inversions(const string& s) {
    int cnt = 0;
    for (size_t i=0; i<s.length(); ++i) {
        for (size_t j=i+1; j<s.length(); ++j) {
            if (s[i] > s[j]) ++cnt;
        }
    }
    return cnt;
}
bool swapaway(const string& s, const string& t) {
    vector<int> diff;
    for (size_t i=0; i<s.length(); ++i) {
        if (s[i] != t[i]) diff.push_back(i);
    }
    if (diff.size() == 2 && s[diff[0]] == t[diff[1]] && s[diff[1]] == t[diff[0]]) return true;
    return false;
}
bool try_improve(int v) {
    if (used[v])
        return false;
    used[v] = true;
    for (int to : e[v]) {
        if (mt[to] == -1 || try_improve(mt[to])) {
            mt[to] = v;
            return true;
        }
    }
    return false;
}
int main() {
    ios::sync_with_stdio(false);
    cin >> n;
    words.resize(n);
    for (int i=0; i<n; ++i) {
        cin >> words[i];
        if (inversions(words[i]) & 1) odd.push_back(i);
        else even.push_back(i);
    }
    p = odd.size(), q = even.size();
    e.assign(p, {});
    for (int i=0; i<p; ++i) {
        for (int j=0; j<q; ++j) {
            if (swapaway(words[odd[i]], words[even[j]])) e[i].push_back(j);
        }
    }

    mt.assign(q, -1);
    for (int v = 0; v < p; ++v) {
        used.assign(p, false);
        try_improve(v);
    }
    int ans = n;
    for (int i = 0; i < q; ++i) {
        if (mt[i] != -1) --ans;
    }
    cout << ans << endl;

    return 0;
}
