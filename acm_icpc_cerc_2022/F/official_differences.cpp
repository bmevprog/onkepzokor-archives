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

#define MAX 100000
#define P 1000003
#define MOD 1000000007

int n,m,k;
vector<string> s;
char line[MAX+1];
int64 p[MAX];

int64 h[MAX][26],c[MAX];

int main() {
	//freopen("test.in","r",stdin);
	scanf("%d %d %d",&n,&m,&k);
	assert(2<=n && n<=100000);
	assert(2<=m && m<=100000);
	assert(n*m<=20000000);
	assert(1<=k && k<=m);
	p[0]=1;
	FOR (i,1,n-1) p[i]=(p[i-1]*P)%MOD;
	s.resize(n);
	REP (i,n) {
		scanf("%s",line);
		s[i]=line;
		REP (j,m) {
			int a=line[j]-'A';
			assert(0<=a && a<4);
			h[j][a]+=p[i]; h[j][a]%=MOD;
			c[j]+=p[i]; c[j]%=MOD;
		}
	}
	int64 tar=0;
	REP (i,n) { tar+=k*p[i]; tar%=MOD; }
	REP (i,n) {
		int64 t=(tar-k*p[i])%MOD;
		if (t<0) t+=MOD;
		int64 f=0;
		REP (j,m) {
			int a=s[i][j]-'A';
			int64 d=c[j]-h[j][a];
			if (d<0) d+=MOD;
			f=(f+d)%MOD;
		}
		if (f==t) cout << i+1 << endl;
	}
	return 0;
}
