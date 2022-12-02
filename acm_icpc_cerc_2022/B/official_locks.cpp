#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <stack>
#include <utility>
#include <fstream>
using namespace std;

#define Assert assert
//#define Assert(x) 

constexpr int MaxCases = 20, MaxN = 10, MaxC = 1000;

int main()
{
    //ifstream ifs("locks99.in"); cin.rdbuf(ifs.rdbuf());
    //ifstream ifs("locks98.in"); cin.rdbuf(ifs.rdbuf());
    //ifstream ifs("locks-tests\\rand04.in"); cin.rdbuf(ifs.rdbuf());
    // Read the number of cases.
    char ch; string line; getline(cin, line); Assert(cin.good());
    int ok_, nCases; ok_ = sscanf(line.c_str(), "%d%c", &nCases, &ch); Assert(ok_ == 1);
    Assert(1 <= nCases); Assert(nCases <= MaxCases);
    while (nCases-- > 0)
    {
        // Read the next case.
        int n, c; getline(cin, line); Assert(cin.good()); 
        ok_ = sscanf(line.c_str(), "%d %d%c", &n, &c, &ch); Assert(ok_ == 2);
        Assert(1 <= n); Assert(n <= MaxN); Assert(0 <= c); Assert(c <= MaxC);
        string s1; getline(cin, s1); Assert(cin.good()); Assert(s1.length() == n);
        string s2; getline(cin, s2); Assert(cin.good()); Assert(s2.length() == n);
        int initialState = 0;
        for (int i = 0; i < n; ++i)
        {
            Assert(s1[i] >= '0'); Assert(s1[i] <= '9');
            Assert(s2[i] >= '0'); Assert(s2[i] <= '9');
            if (s1[i] == s2[i]) initialState |= (1 << i);
        }
        vector<bool> forbidden(1 << n, false); 
        for (int i = 0; i < c; ++i)
        {
            getline(cin, line); Assert(cin.good()); Assert(line.length() == n);
            int state = 0; for (int j = 0; j < n; ++j) { Assert(line[j] == '=' || line[j] == '.'); if (line[j] == '=') state |= (1 << j); }
            Assert(! forbidden[state]); forbidden[state] = true;
        }
        Assert(! forbidden[initialState]); 
        // Nodes u with even parity are left, those with an odd parity are right.
        // We consider edges to be directed and point from left to right; 
        // moreover there is a source S with an edge to every left node, 
        // and a sink T with an edge from every right node.  The capacity of every edge is 1.
        int flow[1 << MaxN][MaxN + 1]; // flow[u][b] = (for left u only) the flow from u to u ^ (1 << b)
            // flow[u][n] = the flow along the edge between u and the source/sink
        bool left[1 << MaxN];
        for (int u = 0; u < (1 << n); ++u)
        {
            int parity = 0; for (int i = 0; i < n; ++i) { flow[u][i] = 0; parity ^= ((u >> i) & 1); }
            left[u] = (parity != 0); flow[u][n] = 0;
        }
        int totalFlow[2] = {0, 0};
        for (int pass = 0; pass < 2; ++pass)
        {
            forbidden[initialState] = (pass == 0);
            if (pass == 1) totalFlow[1] = totalFlow[0];
            // Use the Edmonds-Karp algorithm to find a maximum matching.
            const int S = (1 << n), T = (1 << n) + 1; 
            while (true)
            {
                vector<pair<int, int>> pred((1 << n) + 2, {-1, -1});
                vector<int> toDo; int head = 0;
                // Process edges from the source to left nodes.
                for (int u = 0; u < (1 << n); ++u) if (left[u] && ! forbidden[u] && flow[u][n] == 0) {
                    pred[u] = {S, n}; toDo.emplace_back(u); }
                bool foundAugPath = false;
                while (head < toDo.size())
                {
                    int u = toDo[head++];
                    // Process edges from u to nodes on the opposite side.
                    for (int i = 0; i < n; ++i)
                    {
                        int v = u ^ (1 << i); if (forbidden[v] || pred[v].first >= 0) continue;
                        if (left[u] ? flow[u][i] == 0 : flow[v][i] == 1) { pred[v] = {u, i}; toDo.emplace_back(v); }
                    }
                    if (! left[u] && flow[u][n] == 0)
                    {
                        // We can augment the flow along edge from u to the sink.
                        // This completes an augmenting path.
                        flow[u][n] = 1;
                        while (u != S)
                        {
                            auto [v, i] = pred[u]; 
                            if (left[u]) flow[u][i] ^= 1; else flow[v][i] ^= 1;
                            u = v;
                        }
                        ++totalFlow[pass]; foundAugPath = true; break;
                    }
                }
                if (! foundAugPath) break;
            }
        }
        Assert(totalFlow[0] == totalFlow[1] || totalFlow[1] == totalFlow[0] + 1);
        printf("%s\n", (totalFlow[0] != totalFlow[1] ? "Alice" : "Bob"));
    }
    return 0;
}