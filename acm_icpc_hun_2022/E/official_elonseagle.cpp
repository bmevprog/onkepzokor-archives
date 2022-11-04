#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

const double pi = 3.141592653589793238462643383279502884L;

int n;
struct ray {
    double dir, slope;
};
vector<ray> events;

int main() {
    ios::sync_with_stdio(false);
    cin >> n;
    double t0 = 0.0;
    for (int i=0; i<n; ++i) {
        double t,s,amax; cin >> t >> s >> amax;
        if (s == 0) {
            t0 += t;
            continue;
        }
        double r1 = amax - t/s, r2 = amax + t/s;
        if (r2-r1 < 2*pi) {
            events.push_back({r2, s});
            events.push_back({r1, s});
            events.push_back({amax, -2*s});
            events.push_back({r2+2*pi, s});
            events.push_back({r1+2*pi, s});
            events.push_back({amax+2*pi, -2*s});
        }
        else {
            double amin = amax-pi;
            events.push_back({amin, s});
            events.push_back({amax, -2*s});
            events.push_back({amin+2*pi, 2*s});
            events.push_back({amax+2*pi, -2*s});
            events.push_back({amin+4*pi, s});
            t0 += t - s*pi;
        }
    }
    sort(events.begin(), events.end(), [](const ray& r1, const ray& r2) {
        return r1.dir < r2.dir;
    });
    double currdir = events[0].dir, currslope = 0;
    double tmax = t0;
    for (auto& r : events) {
        t0 += currslope * (r.dir - currdir);
        tmax = max(tmax, t0);
        currslope += r.slope;
        currdir = r.dir;
    }
    cout << fixed << setprecision(8);
    cout << tmax << endl;

    return 0;
}
