#include <iostream>
#include <vector>
#include <set>

using namespace std;

int n, k, root;
vector<int> par, dep, hgt;
vector<vector<int>> e;
vector<bool> active;
vector<int> currhgt;
vector<int> to_add;

void dfs1(int node) {
    hgt[node] = 1;
    for (int ch : e[node]) {
        if (ch == 0) continue;
        dep[ch] = dep[node] + 1;
        dfs1(ch);
        hgt[node] = max(hgt[node], hgt[ch]+1);
    }
}
void dfs2(int node, int hgtlimit) {
    if (node == 0 || currhgt[node] >= hgtlimit) return;
    if (!active[node]) to_add.push_back(node);
    if (hgt[e[node][0]] < hgtlimit-1) {
        dfs2(e[node][0], hgtlimit-2);
        dfs2(e[node][1], hgtlimit-1);
    }
    else {
        dfs2(e[node][0], hgtlimit-1);
        dfs2(e[node][1], hgtlimit-2);
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin >> n >> k;
    par.assign(n+1, -1);
    e.resize(n+1, {0,0});
    for (int i=1; i<=n; ++i) {
        cin >> par[i];
        if (par[i] == -1) root = i;
        else {
            if (i<par[i]) e[par[i]][0] = i;
            else e[par[i]][1] = i;
        }
    }
    dep.assign(n+1,1), hgt.assign(n+1,0);
    dfs1(root);

    active.assign(n+1, false);
    currhgt.assign(n+1, 0);
    int maxdep = n+1;
    set<int> q{root};
    while (k > 0) {
        int nxt = *q.begin();
        q.erase(nxt);
        if (dep[nxt] > maxdep || active[nxt]) continue;
        to_add = { nxt };
        for (int node = nxt; node != root; node = par[node]) {
            int p = par[node];
            int sib = (e[p][0] == node ? e[p][1] : e[p][0]);
            if (currhgt[sib] < currhgt[node]) dfs2(sib, currhgt[node]);
            else if (currhgt[sib] > currhgt[node]) break;
            else continue;
        }
        if ((int)to_add.size() <= k) {
            k -= to_add.size();
            for (int node : to_add) {
                currhgt[node] = 1; active[node] = true;
                for (int c = node, p = par[node]; c != root; p = par[c]) {
                    if (currhgt[p] >= currhgt[c]+1) break;
                    ++currhgt[p];
                    c = p;
                }
                if (e[node][0] > 0) q.insert(e[node][0]);
                if (e[node][1] > 0) q.insert(e[node][1]);
            }
        }
        else maxdep = dep[nxt] - 1;
    }
    for (int i=1; i<=n; ++i) cout << active[i];
    cout << endl;

    return 0;
}
