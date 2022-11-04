#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

int n;
vector<ll> t;
vector<vector<pair<int,ll>>> e;
vector<ll> tsum;
vector<int> sz;
vector<ll> ans;

pair<ll,ll> dfs1(int node, int par = -1) {
    tsum[node] = t[node];
    ll dsum = 0, tdsum = 0;
    for (auto [ch, w] : e[node]) {
        if (ch == par) continue;
        auto ret = dfs1(ch, node);
        sz[node] += sz[ch];
        tsum[node] += tsum[ch];
        dsum += w*sz[ch] + ret.first;
        tdsum += ret.second + w*tsum[ch];
    }
    return {dsum, tdsum};
}
void dfs2(int node, int par, ll dsum, ll tdsum) {
    ans[node] = tdsum + dsum*t[node];
    for (auto [ch, w] : e[node]) {
        if (ch == par) continue;
        ll chdsum = dsum + w*(n-sz[ch]) - w*sz[ch];
        ll chtdsum = tdsum + w*(tsum[0]-tsum[ch]) - w*tsum[ch];
        dfs2(ch, node, chdsum, chtdsum);
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin >> n;
    t.resize(n);
    for (int i=0; i<n; ++i) cin >> t[i];
    e.assign(n, {});
    for (int i=1; i<n; ++i) {
        int x, y, w; cin >> x >> y >> w;
        --x, --y;
        e[x].push_back({y,w});
        e[y].push_back({x,w});
    }
    tsum.assign(n, 0);
    sz.assign(n, 1);
    auto [dsum, tdsum] = dfs1(0);
    ans.assign(n, 0);
    dfs2(0, -1, dsum, tdsum);
    for (int i=0; i<n; ++i) cout << ans[i] << endl;

    return 0;
}
