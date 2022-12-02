#include <iostream>
#include <bits/stdc++.h>
#include <time.h>
#include <random>

using namespace std;
using ll=long long;
using pii=pair<int, int>;
 
vector<pair<long double,int>> bemen;
 
int megold [10001];
 
long double error = 1e-6;
 
bool checkint (long double x)
{
    //cout<<x<<"   1"<<endl;
    if ((roundl(x))-x<error && (roundl(x))-x>-error)
    {
        return true;
    }
    return false;
}
 
int main()
{
    srand(time(0));
    int n;
    cin>>n;
    long double a;
    for (int i = 0; i<n;i++)
    {
        cin>>a;
        bemen.push_back({a,i});
    }
 
 
    sort(bemen.begin(), bemen.end());
    long double elso = bemen.back().first;
    int elsoindex = bemen.back().second;
 
    bemen.pop_back();
 
    //random_shuffle(bemen.begin(), bemen.end());
 
    long double d;
    long double dmegold;
    bool megvan=true;
    for(int i = 1; i <10001;i++)
    {
        megvan=true;
 
        d = i/elso;
 
        for (int j = 0; j <n-1;j++)
        {
            //cout<<bemen[j].first*d<<endl;
            if (!checkint(bemen[j].first*d))
            {
                megvan=false;
                break;
            }
        }
        if (megvan==true)
        {
            dmegold=d;
            break;
        }
    }
 
 
    for (int i = 0; i <n-1;i++)
    {
        megold[bemen[i].second] = roundl(bemen[i].first*dmegold);
    }
    megold[elsoindex] = elso*dmegold;
 
    for (int i =0; i <n;i++)
    {
        cout<<megold[i]<<endl;
    }
}