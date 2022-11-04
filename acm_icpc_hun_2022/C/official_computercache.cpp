#include <iostream>
#include <vector>

using namespace std;

int n,N, m, q;

struct dseg {
    int S;
    vector<int> baseval;
    vector<vector<int>> tree;
    void inc(int l, int r, int time, int node, int lo, int hi) {
        if (l>=hi || r<=lo) return;
        if (l==lo && r==hi) {
            tree[node].push_back(time);
            return;
        }
        int mid = (lo+hi)/2;
        inc(l, min(r,mid), time, 2*node, lo, mid);
        inc(max(l,mid), r, time, 2*node+1, mid, hi);
    }
    int get(int pos, int time, int add, int node, int lo, int hi) {
        add += lower_bound(tree[node].begin(), tree[node].end(), time) - tree[node].begin();
        if (lo+1==hi) return baseval[pos]+add;
        int mid = (lo+hi)/2;
        if (pos<mid) return get(pos, time, add, 2*node, lo, mid);
        else return get(pos, time, add, 2*node+1, mid, hi);
    }
};
vector<dseg> datas;

struct cnode {
    int dataid = 0;
    int datatime = 0;
    int stpos;
};
vector<cnode> cseg;
void update(int l, int r, cnode& cn, int node, int lo, int hi) {
    if (l>=hi || r<=lo) return;
    if (l==lo && r==hi) {
        cseg[node] = cn;
        return;
    }
    int mid = (lo+hi)/2;
    update(l, min(r,mid), cn, 2*node, lo, mid);
    update(max(l,mid), r, cn, 2*node+1, mid, hi);
}
int query(int pos, int node, int lo, int hi) {
    if (lo+1==hi) {
        return datas[cseg[node].dataid].get(pos-cseg[node].stpos, cseg[node].datatime, 0, 1, 0, datas[cseg[node].dataid].S);
    }
    if (cseg[2*node].datatime < cseg[node].datatime) cseg[2*node] = cseg[node];
    if (cseg[2*node+1].datatime < cseg[node].datatime) cseg[2*node+1] = cseg[node];
    int mid = (lo+hi)/2;
    if (pos < mid) return query(pos, 2*node, lo, mid);
    else return query(pos, 2*node+1, mid, hi);
}

int main() {
    ios::sync_with_stdio(false);

    cin >> n >> m >> q;
    for (N=1; N<=n; N<<=1);
    cseg.assign(2*N, cnode());
    for (int i=0; i<n; ++i) cseg[i+N].stpos = i;

    ++m;
    datas.resize(m);
    datas[0].baseval = {0,0};
    datas[0].tree = {{},{},{},{}};
    datas[0].S = 2;
    for (int i=1; i<m; ++i) {
        int s; cin >> s;
        datas[i].baseval.resize(s);
        for (int j=0; j<s; ++j) cin >> datas[i].baseval[j];
        int S = 1;
        for(; S<=s; S<<=1);
        datas[i].S = S;
        datas[i].tree.assign(2*S, {});
    }
    for (int ct=1; ct<=q; ++ct) {
        int type; cin >> type;
        if (type == 1) {
            int i,p; cin >> i >> p; --p;
            cnode cn;
            cn.dataid = i, cn.datatime = ct, cn.stpos = p;
            update(p, p+datas[i].baseval.size(), cn, 1, 0, N);
        }
        else if (type == 2) {
            int p; cin >> p; --p;
            cout << (query(p, 1, 0, N) & 255) << endl;
        }
        else {
            int i,l,r; cin >> i >> l >> r; --l;
            datas[i].inc(l,r,ct,1,0,datas[i].S);
        }
    }

    return 0;
}
