#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int A, B; cin >> A >> B;
    int ans = 0;
    while (A > B) {
        ans += (A&1 ? 2 : 1);
        A = (A+1)/2;
    }
    cout << ans + B - A << endl;
    return 0;
}
