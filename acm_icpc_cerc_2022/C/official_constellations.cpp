#include <bits/stdc++.h>
using namespace std;

#define ALL(c) (c).begin(),(c).end()
#define PB push_back
#define IN(x,c) (find(c.begin(),c.end(),x) != (c).end())
#define REP(i,n) for (int i=0;i<(int)(n);i++)
#define FOR(i,a,b) for (int i=(a);i<=(b);i++)
#define INIT(a,v) memset(a,v,sizeof(a))
template<class A, class B> A cvt(B x) { stringstream ss; ss<<x; A y; ss>>y; return y; }

#define SPC << " " <<
#define DEBUG(x) cerr << #x << " = "; cerr << x << endl;
#define DEBUG_ITER(x) cerr << #x << " = "; for (auto _ : x) cerr << _ << ' '; cerr << endl;

typedef long long int64;
typedef pair<int,int> PII;
typedef vector<int> VI;

int64 sqr(int64 x) { return x*x; }

int n;
int x[2000], y[2000];

struct Const {
	VI stars;
	int valid;
	int64 n, sx1, sx2, sy1, sy2;
};

struct Frac {
	int64 num,den;
};

bool operator<(Frac a, Frac b) {
	return a.num*b.den < b.num*a.den;
}

vector<Const> cs;

using ConstPair = pair<Frac,PII>;
priority_queue<ConstPair, vector<ConstPair>, greater<ConstPair>> pq;

int main() {
	//freopen("constellations.in","r",stdin);
	//freopen("constellations.out","w",stdout);
	cin >> n;
	REP (i,n) {
		cin >> x[i] >> y[i];
		Const c = {{i},1,1,x[i],sqr(x[i]),y[i],sqr(y[i])};
		cs.PB(c);
	}
	REP (i,n) {
		REP (j,i) {
			Frac d = {sqr(x[i]-x[j])+sqr(y[i]-y[j]), 1};
			pq.push({d, {min(i,j),max(i,j)}});
		}
	}
	while (!pq.empty()) {
		auto [d,ij] = pq.top(); pq.pop();
		auto [i,j] = ij;
		if (!cs[i].valid || !cs[j].valid) continue;
		Const c;
		c.stars.insert(c.stars.end(), ALL(cs[i].stars));
		c.stars.insert(c.stars.end(), ALL(cs[j].stars));
		c.valid=1;
		c.n=cs[i].n+cs[j].n;
		c.sx1=cs[i].sx1+cs[j].sx1;
		c.sy1=cs[i].sy1+cs[j].sy1;
		c.sx2=cs[i].sx2+cs[j].sx2;
		c.sy2=cs[i].sy2+cs[j].sy2;
		cs[i].valid = 0;
		cs[j].valid = 0;
		int k=cs.size();
		REP (i,k) {
			Frac f = {cs[i].n*c.sx2 + c.n*cs[i].sx2 - 2*c.sx1*cs[i].sx1 +
			          cs[i].n*c.sy2 + c.n*cs[i].sy2 - 2*c.sy1*cs[i].sy1,
			          cs[i].n*c.n};
			pq.push({f,{min(i,k),max(i,k)}});
		}
		cs.PB(c);
		cout << c.n << endl;
	}
	return 0;
}
