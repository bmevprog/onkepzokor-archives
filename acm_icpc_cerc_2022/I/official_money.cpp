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

// Matrix operations

typedef double Element;
typedef vector<Element> Row;
typedef vector<Row> Matrix;

Matrix zeros(int h, int w) {
    return Matrix(h, Row(w));
}

Matrix identity(int n) {
    Matrix c = zeros(n,n);
    REP (i,n) c[i][i]=1;
    return c;
}

Row operator+(const Row &a, const Row &b) {
    int n=a.size();
    assert(a.size()==b.size());
    Row c(n);
    REP (i,n) c[i]=a[i]+b[i];
    return c;
}

Row operator*(Element k, const Row &a) {
    int n=a.size();
    Row c(n);
    REP (i,n) c[i]=k*a[i];
    return c;
}

Matrix operator*(const Matrix &a, const Matrix &b) {
    int h=a.size(), w=b[0].size();
    assert(a[0].size()==b.size());
    Matrix c = zeros(h,w);
    REP (i,h) REP (j,w) {
        REP (k,a[0].size()) c[i][j]+=a[i][k]*b[k][j];
    }
    return c;
}

Matrix mat_pow(const Matrix &a, int k) {
    assert(a.size()==a[0].size());
    if (k==0) return identity(a.size());
    if (k%2==0) {
        Matrix c=mat_pow(a,k/2);
        return c*c;
    } else {
        Matrix c=mat_pow(a,k-1);
        return c*a;
    }
}

void mat_print(const Matrix &a) {
    REP (i,a.size()) {
        REP (j,a[i].size()) {
            if (j!=0) cout << " ";
            cout << a[i][j];
        }
        cout << endl;
    }
}

// Data

#define N 1000

int c,p,e;
double f[N][N];
vector<int> adj[N], adj_rev[N];
vector<pair<int,double> > out[N];
int k[N];

// SCC - Kosaraju

int vis[N];
vector<int> ord;
vector<int> comp;

void dfs1(int x) {
	vis[x]=1;
	for (int y : adj[x]) if (!vis[y]) dfs1(y);
	ord.PB(x);
}

void dfs2(int x) {
	vis[x]=1;
	comp.PB(x);
	for (int y : adj_rev[x]) if (!vis[y]) dfs2(y);
}

// Main

int main() {
	//freopen("test.in","r",stdin);
	//freopen("test.out","w",stdout);
	cin >> c >> p;
	assert(1<=c && c<=1000);
	REP (i,c) {
		cin >> k[i];
		e+=k[i];
		REP (j,k[i]) {
			char type,colon;
			int x;
			double w;
			cin >> type >> x >> colon >> w;
			x--; w/=100;
			if (type=='P') {
				out[i].PB({x,w});
			} else {
				f[i][x]=w;
				adj[i].PB(x);
				adj_rev[x].PB(i);
			}
		}
	}
	assert(e<=10000);
	Matrix cc = identity(c);
	Matrix pc = zeros(p,c);
	// SCC
	INIT(vis,0);
	REP (i,c) if (!vis[i]) dfs1(i);
	reverse(ALL(ord));
	INIT(vis,0);
	for (int i : ord) if (!vis[i]) {
		comp.clear();
		dfs2(i);
		int n=comp.size();
		assert(n<10);
		// solve component
		Matrix m = zeros(2*n,2*n);
		REP (i,n) {
			int x=comp[i];
			REP (j,n) {
				int y=comp[j];
				m[i][j]=f[y][x];
			}
			m[n+i][i]=1;
			m[n+i][n+i]=1;
		}
		Matrix M = mat_pow(m, 1<<20);
		m = zeros(n,n);
		REP (i,n) REP (j,n) m[i][j]=M[n+i][j];
		Matrix cc2 = zeros(n,c);
		REP (i,n) cc2[i]=cc[comp[i]];
		Matrix acc = m*cc2;
		// distribute out of the component
		REP (i,n) {
			int x=comp[i];
			for (int y : adj[x]) if (!IN(y,comp)) {  // to companies
				cc[y] = cc[y] + f[x][y]*acc[i];
			}
			for (auto [y,w] : out[x]) {  // to people
				pc[y] = pc[y] + w*acc[i];
			}
		}
	}
	REP (i,c) {
		REP (j,p) {
			if (j!=0) printf(" ");
			printf("%.7f",pc[j][i]);
		}
		printf("\n");
	}
	return 0;
}
