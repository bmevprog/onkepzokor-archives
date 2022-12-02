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

int k,j,n;

int f[3][1000001];

int solve(int first, int n) {
	int &r=f[first][n];
	if (r!=-1) return r;
	r=0;
	int a=k-1, b=j-1, i=0;
	if (first==1) a--;
	if (first==2) b--;
	while (a<n || b<n) {
		if (a<b) {
			r+=1;
			i=a+1;
			a+=k;
		} else if (b<a) {
			r+=1;
			i=b+1;
			b+=j;
		} else {
			r+=2;
			int s1=solve(1,n-1-a);
			int s2=solve(2,n-1-a);
			r+=min(s1,s2);
			break;
		}
	}
	return r;
}

int main() {
	cin >> k >> j >> n;
	INIT(f,-1);
	int r = solve(0,n);
	cout << r << endl;
	return 0;
}
