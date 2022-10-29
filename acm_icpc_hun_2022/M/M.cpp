// Az alábbi megoldásért köszönet az Árvíztűrő tükörfúrógép csapatnak! :)

#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>

using namespace std;
using pii = pair<int, long long>;
using ll = long long;

ll n;
ll csucsosszeg = 0;
int root = 0;
vector<ll> al_ossz;
vector<ll> fel_ossz;
vector<ll> al_szor_ossz;
vector<ll> fel_szor_ossz;
vector<ll> al_csucs_ossz;
vector<vector<pii> > g;
vector<ll> ertek;
vector<ll> meret;

void dfs_ut_le(int v, int p)
{
    al_ossz[v] = 0;
    meret[v] = 1;
    for (auto x : g[v])
    {
        if (x.first != p) {
            dfs_ut_le(x.first, v);
            al_ossz[v] += al_ossz[x.first] + x.second*meret[x.first];
            meret[v] += meret[x.first];
        }
    }
}

void dfs_ut_fel(int v, int p, ll w)
{
    if (v == root) {
        fel_ossz[v] = 0;
    }
    else {
        fel_ossz[v] = fel_ossz[p] + al_ossz[p] - al_ossz[v] - meret[v] * w + (n - meret[v]) * w;
    }
    for (auto x : g[v])
    {
        if (x.first != p) {
            dfs_ut_fel(x.first, v, x.second);
        }
    }
}

void dfs_szor_le(int v, int p)
{
    al_szor_ossz[v] = 0;
    al_csucs_ossz[v] = ertek[v];
    for (auto x : g[v])
    {
        if (x.first != p) {
            dfs_szor_le(x.first, v);
            al_szor_ossz[v] += x.second * al_csucs_ossz[x.first] + al_szor_ossz[x.first];
            al_csucs_ossz[v] += al_csucs_ossz[x.first];
        }
    }
}

void dfs_szor_fel(int v, int p, ll w)
{
    if (v == root) {
        fel_szor_ossz[v] = 0;
    }
    else {
        fel_szor_ossz[v] = fel_szor_ossz[p] + (csucsosszeg - al_csucs_ossz[p] + ertek[p]) * w +
            al_szor_ossz[p] - w * al_csucs_ossz[v] - al_szor_ossz[v] +
            (al_csucs_ossz[p] - al_csucs_ossz[v]) * w -
            ertek[p] * w;
    }
    for (auto x : g[v])
    {
        if (x.first != p) {
            dfs_szor_fel(x.first, v, x.second);
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    cin >> n;
    al_ossz.resize(n);
    fel_ossz.resize(n);
    al_csucs_ossz.resize(n);
    al_szor_ossz.resize(n);
    fel_szor_ossz.resize(n);
    ertek.resize(n);
    meret.resize(n);
    g.assign(n, vector<pii>());
    for (int i = 0; i < n; i++)
    {
        cin >> ertek[i];
        csucsosszeg += ertek[i];
    }
    //cout << csucsosszeg << endl;
    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        u--; v--;
        g[u].push_back({ v, w });
        g[v].push_back({ u, w });
    }
    dfs_ut_le(root, root);
    dfs_ut_fel(root, root, 0);
    dfs_szor_le(root, root);
    dfs_szor_fel(root, root, 0);
    for (int i = 0; i < n; i++)
    {
        ll v1 = al_szor_ossz[i] + fel_szor_ossz[i];
        ll v2 = ertek[i] * (al_ossz[i] + fel_ossz[i]);
        cout << v1 + v2 << "\n";
    }
    return 0;
}
/*
5
2 5 3 4 1
1 2 2
2 4 5
4 3 3
5 2 6

6
4 3 3 4 3 3
1 3 2
2 1 1
1 4 6
4 5 6
6 4 2


*/