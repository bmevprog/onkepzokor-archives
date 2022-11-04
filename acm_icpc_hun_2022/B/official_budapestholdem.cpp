#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

string str;
vector<int> cnt(255,0);

int main() {
    ios::sync_with_stdio(false);
    for (int i=0; i<5; ++i) {
        cin >> str;
        ++cnt[str[0]];
    }
    cout << *max_element(cnt.begin(), cnt.end()) << endl;
    return 0;
}
