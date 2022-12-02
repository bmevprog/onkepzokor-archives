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

string s,t,p;

vector<int> KMP_init(string &v) {
	int m=v.size();
	int i,j;
	vector<int> f(m+1);
	f[0]=0; f[1]=0;
	i=2; j=0;
	while (i<=m) {
		if (v[j]==v[i-1]) { j++; f[i]=j; i++;  }
		else if (j==0) { f[i]=0; i++; }
		else { j=f[j]; }
	}
	return f;
}

vector<int> KMP_search(vector<int> &f, string &v, string &b) {
	int m=v.size(), n=b.size();
	vector<int> r(n);
	int i=0, j=0;
	while (i<n) {
		if (v[j]==b[i]) {
			r[i]=++j;
			if (j==m) j=f[j];
			i++;
		} else if (j==0) { r[i]=0; i++; }
		else { j=f[j]; }
	}
	return r;
}

vector<int> z_algorithm(string &s) {
	int n=s.size();
	vector<int> z(n);
	z[0]=0;
	for (int i=1, l=0, r=0; i<n; i++) {
		if (i<=r) z[i]=min(r-i+1, z[i-l]);
		else z[i]=0;
		while (i+z[i]<n && s[z[i]]==s[i+z[i]]) z[i]++;
		if (i+z[i]-1>r) { l=i; r=i+z[i]-1; }
	}
	return z;
}

vector<int> lcp(string &v, string &b) {
	string s=v+"#"+b;
	auto z = z_algorithm(s);
	return vector<int>(z.begin()+v.size()+1, z.end());
}

int solve(int i, int j) {
	return 0;
}


int main() {
	//freopen("test.in","r",stdin);

	cin >> s >> t >> p;
	int ns=s.size(), nt=t.size(), np=p.size();

	string sr=s; reverse(ALL(sr));
	string tr=t; reverse(ALL(tr));
	string pr=p; reverse(ALL(pr));

	// longest prefix of t starting at i in p (for matches of p extending over entire t)
	auto lp_t_p = lcp(t,p);
	// longest prefix of p starting at i in s/t/p
	auto lp_p_s = lcp(p,s);
	auto lp_p_t = lcp(p,t);
	auto lp_p_p = lcp(p,p);
	// longest suffix of p ending at i in s/t/p
	auto ls_p_s = lcp(pr,sr); REP(i,ns/2) swap(ls_p_s[i],ls_p_s[ns-1-i]);
	auto ls_p_t = lcp(pr,tr); REP(i,nt/2) swap(ls_p_t[i],ls_p_t[nt-1-i]);
	auto ls_p_p = lcp(pr,pr); REP(i,np/2) swap(ls_p_p[i],ls_p_p[np-1-i]);
	// locations of partial matches of p at the front and back of t
	vector<int> front_t(nt), back_t(nt);
	REP (i,nt) {
		front_t[i]=(ls_p_t[i]==i+1);
		back_t[i]=(lp_p_t[i]==nt-i);
	}

	// initialize KMP failure functions
	vector<int> f_p = KMP_init(p);
	vector<int> f_pr = KMP_init(pr);
	// longest preffix of p ending at index i in s/t
	auto pre_s=KMP_search(f_p,p,s);
	auto pre_t=KMP_search(f_p,p,t);
	// longest suffix of p starting at index i in s/t
	auto suf_s=KMP_search(f_pr,pr,sr); REP(i,ns/2) swap(suf_s[i],suf_s[ns-1-i]);
	auto suf_t=KMP_search(f_pr,pr,tr); REP(i,nt/2) swap(suf_t[i],suf_t[nt-1-i]);
	// starting index of match of p in s/t
	vector<int> start_s(ns), start_t(nt);
	int match_s=0, match_t=0;
	REP (i,ns) if (pre_s[i]==np) { start_s[i-np+1]=1; match_s++; }
	REP (i,nt) if (pre_t[i]==np) { start_t[i-np+1]=1; match_t++; }

	vector<int> cum_s(ns+1);  // cumulative number of p starting in the first i characters of s
	cum_s[0]=0;
	REP (i,ns) cum_s[i+1]=cum_s[i]+start_s[i];

	vector<int> front_matches(np);  // matches at the front of t for a given maximum prefix i of p before t
	for (int i=1;i<np;i++) {
		front_matches[i]=front_matches[f_p[i]];
		if (np-i<nt) front_matches[i]+=front_t[np-i-1];
	}
	vector<int> back_matches(np);  // matches at the back of t for a given maximum suffix i of p after t
	for (int i=1;i<np;i++) {
		back_matches[i]=back_matches[f_pr[i]];
		if (np-i<nt) back_matches[i]+=back_t[nt-(np-i)];
	}

	// match of p with prefix of exactly i, entire t and suffix of exactly j
	auto match_ij = [&] (int i, int j) {
		return i+nt+j==np && lp_t_p[i]==nt;
	};
	// match of p with prefix of exactly i, entire t and suffix of at most j
	auto match_i = [&] (int i, int j) {
		int d=np-i-nt;
		return d>=0 && i+nt+j>=np && lp_t_p[i]==nt && ls_p_p[np-j+d-1]>=d;
	};
	// match of p with prefix of at most i, entire t and suffix of exactly j
	auto match_j = [&] (int i, int j) {
		int d=np-j-nt;
		return d>=0 && i+nt+j>=np && lp_t_p[np-j-nt]==nt && lp_p_p[i-d]>=d;
	};
	// mark special nodes
	double lim=sqrt(np);
	vector<int> sz(np+1,1), mark(np+1);
	for (int i=np;i>0;i--) {
		if (sz[i]<lim) sz[f_p[i]]+=sz[i];
		else mark[i]=1;
	}
	mark[0]=1;
	// solve subproblem g(i,0)
	vector<int> g(np+1);
	g[0] = (t==p);
	for (int i=1;i<=np;i++) {
		g[i] = g[f_p[i]] + match_ij(i,0);
	}
	// solve subproblems h(i',j) for marked nodes i'
	vector<vector<int>> h(np+1);
	for (int i=0;i<=np;i++) if (mark[i]) {
		h[i]=vector<int>(np+1);
		h[i][0]=g[i];
		for (int j=1;j<=np;j++) {
			h[i][j]=h[i][f_pr[j]]+match_j(i,j);
		}
	}
	// answer queries f(i,j)
	auto solve = [&] (int i, int j) {
		int f=0;
		while (!mark[i]) {
			f+=match_i(i,j);
			i=f_p[i];
		}
		return f+h[i][j];
	};

	// check all insertion points after k chars in s
	int best=0, cnt=0, left=-1, right=-1;
	REP (k,ns+1) {
		int match=match_s+match_t;
		// subtract broken
		match-=cum_s[k]-cum_s[max(k-np+1,0)];
		if (k>0) {  // add newly formed at the start of t
			int i=pre_s[k-1];
			if (i==np) i=f_p[i];
			match+=front_matches[i];
		}
		if (k<ns) {  // add newly formed at the end of t
			int i=suf_s[k];
			if (i==np) i=f_pr[i];
			match+=back_matches[i];
		}
		// expanding over entire t
		int i=(k>0)?pre_s[k-1]:0, j=(k<ns)?suf_s[k]:0;
		if (i==np) i=f_p[i];
		if (j==np) j=f_pr[j];
		match+=solve(i,j);
		if (t==p) match--;

		// update results
		if (match>best) { best=match; cnt=0; left=-1; }
		if (match==best) {
			cnt++;
			if (left==-1) left=k;
			right=k;
		}
	}
	cout << best << " " << cnt << " " << left << " " << right << endl;
	return 0;
}
