#include <iostream>
#include <queue>
#include <cmath>
using namespace std;

//Max num of vertices.
#define N 7

//Edmonds-Karp algorithm.
struct MaxFlowMinCut {

//Set by hand before calling init().
	
	//Actual num of vertices
	int n;
	
	//Original directed capacity matrix.
	int co[N][N];

	//Source
	int s;

	//Target
	int t;


//Set by init().
	
	//Current directed capacity matrix.
	int cc[N][N];

	//Current flow
	int f[N][N];

	//Max flow found.
	int maxflow;

//Set by BFS().

	//Parent table
	int p[N];
	
	//Flow on the path
	int flow;

	void init() {

		maxflow = 0;

		for(int i=0; i<n; ++i) for(int j=0; j<n; ++j) {
			cc[i][j] = co[i][j];
			f[i][j] = 0;
		}		
	}

	void calculate() {
		
		while(BFS()) {
			maxflow +=flow;
			int curr = t;
			int par;
			while(curr!=s) {
				par = p[curr];
				cc[par][curr] = cc[par][curr] - flow;
				cc[curr][par] = cc[curr][par] + flow;
	
				f[par][curr] = f[par][curr] + flow;

				curr = par;
			}
		}
		//In p[N] now anybody whose parent is not -1 is in the cut part of the graph.
		//Anybody whose p[i] is not -1 AND is not listed as a parent is on the edge of the cut.
	}


	bool BFS() {

		int m[n];

		for(int i=0; i<n; ++i) {
			p[i] = -1;
			m[i] = 0;
		}

		m[s] = -2;
		p[s] = -2;

		queue<int> frontier;

		frontier.push(s);

		while(!frontier.empty()) {

			int curr = frontier.front();
			frontier.pop();

			for(int i=0; i<n; ++i) if(cc[curr][i]>0 && p[i] == -1) {
				
				p[i] = curr;
				m[i] = (m[curr] == -2) ?  cc[curr][i] : min(cc[curr][i], m[curr]);
				
				if(i==t) {
					flow = m[t];
					return true;
				} else {
					frontier.push(i);	
				}
			}
		}
		flow=0;
		return false;
	}

};

int main () {
	MaxFlowMinCut mfmc;
	
	cin>>mfmc.s>>mfmc.t>>mfmc.n;

	for(int i=0; i<mfmc.n; ++i) for(int j=0; j<mfmc.n; ++j)
		cin>>mfmc.co[i][j];

	mfmc.init();
	mfmc.calculate();
	cout<<mfmc.maxflow<<endl;

	for(int i=0; i<mfmc.n; ++i) if(mfmc.p[i]!=-1) cout<<(char)('A'+i)<<' ';
	cout<<endl;
	cout<<endl;

	return 0;
}
