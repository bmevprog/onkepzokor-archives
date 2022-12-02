#include <iostream>
#include <iomanip>
#include <climits>
#include <stack>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <cassert>

#define FOR(i,n) for(int i=0,_n=n;i<_n;i++)
#define FORR(i,s,n) for(int i=s,_n=n;i<_n;i++)
#define mp make_pair
#define pb push_back
#define pii pair<int,int>
#define pli pair<ll,int>
#define vi vector<int>
#define fs first
#define sec second

#define maxn 1000000
#define infty 2000000

using namespace std;
typedef long long ll;

const ll MOD = 1000000007LL;

int k,j,n;
int len_AB=-1, len_BA=-1;

int gcd(int a, int b, int& x, int& y) {
    if (b == 0) {
        x = 1,y = 0;
        return a;
    }
    int x1, y1;
    int d = gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

bool diophant(int a, int b, int c, int &x0, int &y0, int &g) {
    g = gcd(abs(a), abs(b), x0, y0);
    if (c % g) {
        return false;
    }
    x0 *= c / g;
    y0 *= c / g;
    if (a < 0) x0 = -x0;
    if (b < 0) y0 = -y0;
    return true;
}

int solve(int n){
    int result = 0;
    int free_AB = k-1-n%k;
    if(free_AB * len_AB >= n)return n/j+(n+1)/k;
    else {
        n -= free_AB*len_AB;
        result += free_AB*len_AB/j+(free_AB*len_AB+free_AB)/k;
    }
    int free_BA = j-1-n%j;
    if(free_BA * len_BA >= n)return result + (n+1)/j+n/k;
    else {
        n -= free_BA*len_BA;
        result += free_BA*len_BA/k+(free_BA*len_BA+free_BA)/j;
    }
    if(k*len_AB>j*len_BA)
        return result + n/j + n/k + n/(k*len_AB)+1;
    else
        return result + n/j + n/k + n/(j*len_BA)+1;
}

int main(){
    scanf("%d%d%d",&k,&j,&n);
    int x, y, g;
    int first_collision = k*(j/gcd(k,j, x, y));
    if(first_collision > n){
        printf("%d\n",n/k+n/j);
        return 0;
    }
    n-=first_collision;
    int partial_ans = first_collision/k + first_collision/j, best_ans=infty;
    //A first: x*k = 1+y*j --> x*k-y*j = 1
    if (diophant(k,-j,1,x,y,g)){
        while(x<0 || y<0)x+=j/g,y+=k/g;
        len_AB = y*j;
    } else best_ans = partial_ans+n/j+(n+1)/k;
    //B first: 1+x*k = y*j --> -k*x+y*j = 1
    if (diophant(-k,j,1,x,y,g)){
        while(x<0 || y<0)x+=j/g,y+=k/g;
        len_BA = x*k;
    } else best_ans = min(best_ans,partial_ans+(n+1)/j+n/k);
    if(best_ans == infty)
        best_ans = solve(n) + partial_ans;
    printf("%d\n", best_ans);
	return 0;
}
