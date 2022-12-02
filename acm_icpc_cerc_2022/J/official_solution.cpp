#include <cstdio>
#include <cmath>
#include <vector>

#define FOR(i,n) for(int i=0,_n=n;i<_n;i++)
#define FORR(i,s,n) for(int i=s,_n=n;i<_n;i++)
#define mp make_pair
#define pb push_back
#define pil pair<int,ll>
#define vi vector<int>
#define fs first
#define sec second

#define maxn 200000
#define infty 1000000000

using namespace std;
typedef long long ll;

const ll MOD = 1000000007LL;

int n,m;
int a[maxn];

struct Node{
    int l;
    int r;
    ll s;
    vector<pil> lines; //{slope, constant}
    vector<double> x_coord; 
    vi hull;
};

Node stree[maxn*4];

void setup(int i, int l, int r){
    stree[i].l = l;
    stree[i].r = r;
    if (l==r){
        stree[i].lines.pb(mp(-1,a[l]));
        stree[i].hull.pb(0);
        stree[i].s=a[l];
        return;
    }
    int mid = (l+r)/2;
    setup(i*2, l, mid);
    setup(i*2+1, mid+1, r);
    stree[i].s = 0;
    FORR(j,l,r+1){
        stree[i].s += a[j];
        stree[i].lines.pb(mp(-(j-(l-1)), stree[i].s));
    }
    stree[i].hull.pb(0);
    FORR(j,1 ,r-l+1){
        while(1){
            int prev = stree[i].hull.back();
            double k1 = stree[i].lines[prev].fs;
            double n1 = stree[i].lines[prev].sec;
            double k2 = stree[i].lines[j].fs;
            double n2 = stree[i].lines[j].sec;
            //|k1-k2|>=1, no precision problems
            double col_x = (n2-n1)/(k1-k2);
            if(stree[i].x_coord.size() == 0 || stree[i].x_coord.back() < col_x){
                stree[i].x_coord.pb(col_x);
                stree[i].hull.pb(j);
                break;
            }
            stree[i].hull.pop_back();
            stree[i].x_coord.pop_back();
        }
    }
}

double query(int i, int l, int r, ll & s){
    if(stree[i].r<l || r<stree[i].l){
        return infty;
    }
    if(stree[i].l>=l && stree[i].r<=r){
        int p1=0, p2=stree[i].hull.size()-1;
        int cnt = stree[i].l-l;
        while (p1<p2){
            int mid = (p1+p2)/2;
            pil line = stree[i].lines[stree[i].hull[mid]];
            //s-cnt*x + l.fs*x + l.sec > 0
            //line.fs<0 and cnt>=0, so line.fs-cnt cannot be 0
            double x = -(double)(line.sec+s)/(double)(line.fs-cnt);
            if (mid+1 < stree[i].hull.size() && x > stree[i].x_coord[mid])p1=mid+1;
            else if (mid>0 && x<stree[i].x_coord[mid-1])p2=mid;
            else p1=p2=mid;
        }
        pil line = stree[i].lines[stree[i].hull[p1]];
        double x = -(double)(line.sec+s)/(double)(line.fs-cnt);
        s+=stree[i].s;
        return x;
    }
    double res1 = query(i*2,l,r,s);
    double res2 = query(i*2+1,l,r,s);
    return min(res1, res2);
}


int main(){
    scanf("%d%d",&n,&m);
    FOR(i,n)scanf("%d",a+i);
    setup(1,0,n-1);
    FOR(cnt,m){
        int l,r;
        scanf("%d%d",&l,&r);
        l--,r--;
        r+=l;
        ll s=0;
        double sol = query(1, l, r, s);
        if (sol<0)printf("stay with parents\n");
        else printf("%d\n",int(sol));
    }
	return 0;
}
