#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cassert>
#include <random>
#include <algorithm>
#include <stack>
#include <utility>
using namespace std;

#define Assert assert
//#define Assert(x) 

int main()
{
    //freopen("permutations98.in", "rt", stdin);
    //freopen("permutations-tests\\permutacije10.in", "rt", stdin);
    // Read the inversion table.
    int n; int ok_ = scanf("%d", &n); Assert(ok_ == 1);
    constexpr int MaxN = 100'000;
    Assert(1 <= n); Assert(n <= MaxN);
    vector<int> B(n, -1); for (int i = 0; i < n; ++i) {
        ok_ = scanf("%d", &B[i]); Assert(ok_ == 1);
        Assert(0 <= B[i]); Assert(B[i] < n - i); }
    // Initialize the permutation and the segment tree.
    // Initially all the slots are empty (= set to -1).
    vector<int> perm(n, -1); 
    // nEmpty[k][j] = number of empty slots in perm[(j << k)..((j + 1) << k) - 1]
    vector<vector<int>> nEmpty;
    nEmpty.emplace_back(n, 1);
    while (nEmpty.back().size() > 1) 
        nEmpty.emplace_back(nEmpty.back().size() / 2, 1 << nEmpty.size());
    // Process the inversion table b_i in increasing order of i.
    for (int i = 0; i < n; ++i)
    {
        // We have to place the element 'i' into the 'bi'th empty slot of the permutation.
        // We'll use the segment tree to find where this empty slot is.
        int bi = B[i] + 1, k = nEmpty.size() - 1, j = 0;
        while (k >= 0)
        {
            j *= 2; 
            if (j < nEmpty[k].size() && bi > nEmpty[k][j]) bi -= nEmpty[k][j++];
            if (j < nEmpty[k].size()) --nEmpty[k][j];
            k--;
        }
        Assert(bi == 1);
        Assert(perm[j] < 0); 
        perm[j] = i;
    }
    // Write the permutation as a sequence of cycles.
    for (int i = 0; i < n; ++i) if (perm[i] >= 0)
    {
        if (i > 0) printf(" " );
        for (int j = i; ; )
        {
            printf("%c%d", (j == i) ? '(' : ' ', j + 1);
            int next = perm[j]; perm[j] = -1;
            if (next == i) break; else j = next;
        }
        printf(")");
    }
    printf("\n");
    return 0;
}