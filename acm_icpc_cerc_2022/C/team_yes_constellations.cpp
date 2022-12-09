#include <bits/stdc++.h>

using ll = long long;
using namespace std;


struct option
{
    ll sum;
    int a, b;
    ll sizea, sizeb;

    option(ll sum, int a, int b, int sizea, int sizeb) : sum(sum), a(a), b(b), sizea(sizea), sizeb(sizeb) {}
    bool operator < (const option& o) const
    {
        return make_pair(sum*o.sizea*o.sizeb, make_pair(a, b)) > make_pair(o.sum*sizea*sizeb, make_pair(o.a, o.b));
    }
};


int main()
{
    int n;
    cin>>n;
    vector<ll> inp_x(n), inp_y(n);
    for(int i=0;i<n;i++) cin>>inp_x[i]>>inp_y[i];

    vector<vector<ll> > dist_matrix(n, vector<ll>(n, -1));
    vector<ll> group_sizes(n, 1);
    vector<bool> in_play(n, true);
    priority_queue<option> pq;

    for(int i=0;i<n;i++)
    {
        for(int j=i+1;j<n;j++)
        {
            dist_matrix[i][j] = dist_matrix[j][i] = (inp_x[i]-inp_x[j])*(inp_x[i]-inp_x[j])+(inp_y[i]-inp_y[j])*(inp_y[i]-inp_y[j]);
            pq.push(option(dist_matrix[i][j], i, j, 1, 1));
        }
    }

    while(!pq.empty())
    {
        option o = pq.top();
        pq.pop();
        if(!in_play[o.a] || !in_play[o.b]) continue;
        in_play[o.a] = in_play[o.b] = false;

        in_play.push_back(true);
        group_sizes.push_back(group_sizes[o.a]+group_sizes[o.b]);
        dist_matrix.push_back(vector<ll>());

        for(int i=0;i<dist_matrix.size()-1;i++)
        {
            if(!in_play[i]) dist_matrix.back().push_back(-1);
            else
            {
                ll curr_dist = (dist_matrix[o.a][i]+dist_matrix[o.b][i]);
                dist_matrix.back().push_back(curr_dist);
                dist_matrix[i].push_back(curr_dist);
                pq.push(option(curr_dist, i, dist_matrix.size()-1, group_sizes[i], group_sizes.back()));
            }
        }
        dist_matrix.back().push_back(0);

        cout<<group_sizes.back()<<endl;
    }
    return 0;
}
