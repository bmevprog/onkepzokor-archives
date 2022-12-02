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

int w;

int id=0;
int m[100000];
int n[100000];
vector<int> br[100000];

int64 total=0;

void read() {
	int x=id++;
	cin >> m[x] >> n[x];
	total+=m[x];
	REP (i,n[x]) {
		br[x].PB(id);
		read();
	}
}

PII solve(int x) {
	int k=0;
	VI stubs;
	int sum=0;
	for (int y : br[x]) {
		auto [ny,my] = solve(y);
		k+=ny;
		sum+=my;
		stubs.PB(my);
	}
	// cut off largest branches
	sort(ALL(stubs));
	while (sum>w) {
		int my=stubs.back();
		stubs.pop_back();
		k++;
		sum-=my;
	}
	// trim the trunk
	m[x]+=sum;
	return {k+m[x]/w, m[x]%w};
}

int main() {
	//freopen("test.in","r",stdin);
	cin >> w;
	read();
	assert(id<=1e5);
	assert(total<=1e9);
	PII sol = solve(0);
	int k = sol.first + (sol.second!=0);
	cout << k << endl;
	return 0;
}
