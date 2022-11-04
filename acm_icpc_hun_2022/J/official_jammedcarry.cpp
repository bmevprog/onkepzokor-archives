#include <iostream>
#include <array>

using namespace std;
using num = array<int,25>;

num square(const num& x) {
    num xsq;
    for (int k=0; k<25; ++k) {
        xsq[k] = 0;
        for (int i=0; i<=k; ++i) {
            xsq[k] += (x[i]*x[k-i]) % 10;
        }
        xsq[k] %= 10;
    }
    return xsq;
}
bool parteq(const num& x, const num& y, int frompos = 24) {
    for (int k=0; k<=frompos; ++k) {
        if (x[k] != y[k]) return false;
    }
    return true;
}
bool operator<(const num& x, const num& y) {
    for (int k=24; k>=0; --k) {
        if (x[k] != y[k]) return x[k] < y[k];
    }
    return false;
}
void print(const num& x, int frompos = 24) {
    for (int k=frompos; k>=0; --k) cout << x[k];
    cout << endl;
}


int l;
num n, a, besta, INF;

void rec(int pos) {
    if (2*pos >= l) {
        if (parteq(n, square(a)) && a < besta) besta = a;
        return;
    }
    for (int d=0; d<10; ++d) {
        a[pos] = d;
        if (parteq(n, square(a), pos)) rec(pos+1);
    }
    a[pos] = 0;
}
int main() {
    ios::sync_with_stdio(false);
    string str; cin >> str;
    l = str.size();
    if (l % 2 == 0) {
        cout << -1 << endl;
        return 0;
    }
    for (int k=0; k<25; ++k) {
        n[k] = (k<l ? str[l-k-1]-'0' : 0);
        besta[k] = INF[k] = 9;
    }
    rec(0);
    if (parteq(besta, INF)) cout << -1 << endl;
    else print(besta, l/2);
    
    return 0;
}
