#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

vector<double> td(4);
vector<int> t(4);
int target;

int main() {
    ios::sync_with_stdio(false);
    for (int i=0; i<4; ++i) {
        cin >> td[i];
        t[i] = (int)round(100.0*td[i]);
    }
    sort(t.begin(), t.end());
    double tr; cin >> tr;
    target = (int)round(100.0*tr);
    if (t[1]+t[2]+t[3] <= 3*target) {
        cout << "infinite" << endl;
    }
    else if (t[0]+t[1]+t[2] > 3*target) {
        cout << "impossible" << endl;
    }
    else {
        int ans = 3*target - t[1] - t[2];
        int frac = ans%100;
        cout << ans/100 << (frac<10?".0":".") << ans%100 << endl;
    }

    return 0;
}
