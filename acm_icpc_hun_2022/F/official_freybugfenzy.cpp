#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

int n,k;
vector<double> p;
vector<double> overk;

int main() {
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    cin >> n >> k;
    p.resize(n);
    for (int i=0; i<n; ++i) cin >> p[i];
    overk.assign(n, 0.0);
    overk[k-1] = 1.0;
    for (int i=k; i<n; ++i) {
        overk[i] = overk[i-1] * (1.0*i)/(1.0*(i-k+1));
    }
    double denom = overk[n-1]*(1.0*n)/(1.0*k);
    for (int i=0; i<n; ++i) {
        double nom = overk[n-1] * p[i];
        for (int d = 1; d <= n-1; ++d) {
            int j = (i-d+n) % n;
            nom += overk[n-1-d] * p[j];
        }
        cout << nom/denom << " ";
    }
    cout << endl;

    return 0;
}
