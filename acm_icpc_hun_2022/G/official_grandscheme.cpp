#include <iostream>
#include <vector>
#include <map>

using namespace std;

int n, m;
map<int,int> nums2id;
vector<int> id2nums;
vector<vector<int>> e;
vector<int> par;

void dfs(int node) {
    for (int ch : e[node]) {
        if (par[ch] != -1) continue;
        par[ch] = node;
        dfs(ch);
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin >> n;
    e.assign(n, {});
    id2nums.resize(n);
    m = n;
    for (int i=0; i<n; ++i) {
        id2nums[i] = i+1;
        int k; cin >> k;
        for (int j=0; j<k; ++j) {
            int x; cin >> x;
            if (nums2id.count(x)) {
                e[i].push_back(nums2id[x]);
                e[nums2id[x]].push_back(i);
            }
            else {
                nums2id[x] = m;
                id2nums.push_back(x);
                e.push_back({i});
                e[i].push_back(m);
                ++m;
            }
        }
    }
    par.assign(m, -1);
    par[0] = 0;
    dfs(0);
    vector<vector<int>> ans;
    for (int i=1; i<n; ++i) {
        if (par[i] == -1) {
            cout << "impossible" << endl;
            return 0;
        }
        ans.push_back({id2nums[par[par[i]]], id2nums[i], id2nums[par[i]]});
    }
    for (auto& xx : ans) cout << xx[0] << " " << xx[1] << " " << xx[2] << endl;

    return 0;
}
