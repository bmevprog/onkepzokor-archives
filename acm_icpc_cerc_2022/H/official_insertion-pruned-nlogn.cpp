#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <utility>
using namespace std;

// Given a string p[0..n-1], this function calculates, for k = 0..n,
// v[k] = the greatest i < k such that p[:i] is a suffix of p[:k].
template<typename String>
void BuildSuffixTable(const String &p, int n, vector<int> &v)
{
    v.resize(n + 1); v[0] = -1; if (n > 0) v[1] = 0;
    for (int k = 2; k <= n; ++k)
    {
        // If p[:i] is a suffix of p[:k], then p[:i-1] must be a suffix of p[:k-1] and p[i-1] must equal p[k-1].
        int j = v[k - 1]; // j is the current candidate for i-1
        while (j > 0 && p[k - 1] != p[j]) 
            // If the candidate j doesn't fit, try the next shorter prefix of p that is also a sufifx of p[:k-1].
            // This algorithm is O(n) because k-j keeps increasing over all the executions of the inner loop.
            j = v[j];
        v[k] = (p[k - 1] == p[j]) ? j + 1 : 0;
    }
}

// The following function computes, for the string s[0..sd-1], its suffix table with respect to p[0..pd-1],
// i.e. v[k] = the greatest i < pd (and i <= k) such that p[:i] is a suffix of s[:k].
// pST[k] must be the greatest i < k such that p[:i] is a suffix of p[:k].
template<typename String>
void BuildSuffixTable2(const String &s, int sd, const String &p, int pd, const vector<int> &pST, vector<int> &v)
{
    v.resize(sd + 1); v[0] = 0; 
    for (int k = 1; k <= sd; ++k)
    {
        // If p[:i] is a suffix of s[:k], then p[:i-1] must be a suffix of s[:k-1] and p[i-1] must equal s[k-1].
        int j = v[k - 1]; // j is the current candidate for i-1
        while (j > 0 && ! (j < pd - 1 && s[k - 1] == p[j]))
            // If the candidate j doesn't fit, try the next shorter prefix of p that is also a suffix of s[:k-1].
            // This algorithm is O(n) because k-j keeps increasing over all the executions of the inner loop.
            j = pST[j];
        v[k] = (j < pd - 1 && s[k - 1] == p[j]) ? j + 1 : 0;
    }
}

// For k = 0..sd, returns v[k] = number of occurrences of p in s[:k].
// spST[k] must be the greatest i < min(pd, k) such that p[:i] is a suffix of s[:k].
template<typename String>
void BuildOccCounts(const String &s, int sd, const vector<int> &spST, const String &p, int pd, vector<int> &v)
{
    v.resize(sd + 1); v[0] = 0;
    const auto pLast = p[pd - 1];
    for (int k = 1; k <= sd; ++k)
        v[k] = v[k - 1] + (spST[k - 1] == pd - 1 && s[k - 1] == pLast ? 1 : 0);
}

// Returns v[k] = the least i > k such that p[i:] is a prefix of p[k:].
// pRST should be the result of calling BuildSuffixTable(p.rbegin(), pd, pRST); 
void BuildPrefixTable(const vector<int> pRST, vector<int> &v)
{
    const int pd = pRST.size() - 1;
    v = pRST;
    // v[k] = the greatest i < k such that p^R[:i] is a suffix of p^R[:k],
    //                      i.e. such that (p[pd-i:])^R is a suffix of (p[pd-k:])^R,
    //                      i.e. such that p[pd-i:] is a prefix of p[pd-k:].
    reverse(v.begin(), v.end());
    // v[pd - k] = the greatest  i < k such that p[pd-i:] is a prefix of p[pd-k:]
    // i.e. v[k] = the greatest  i < pd - k such that p[pd-i:] is a prefix of p[k:]
    for (auto &x : v) x = pd - x;
    // v[k] = the greatest pd - i < pd - k such that p[i:] is a prefix of p[k:]
    // i.e. v[k] = the least i > k such that p[i:] is a prefix of p[k:]
}

// Given tpRST[k] = the greatest i < pd (and i <= k) such that p^R[:i] is a suffix of t^R[:k],
// this function calculates v[k] = the least i > 0 such that p[i:] is a prefix of t[k:].
void BuildPrefixTable2(int pd, const vector<int> &tpRST, vector<int> &v)
{
    // tpRST[k] = the greatest i < pd (and i <= k) such that p^R[:i] is a suffix of t^R[:k]
    //                                       i.e. such that (p[pd-i:])^R is a suffix of (t[td-k:])^R
    //                                       i.e. such that p[pd-i:] is a prefix of t[td-k:].
    v = tpRST;
    reverse(v.begin(), v.end());
    // v[k] = the greatest i < pd (and i <= td-k) such that p[pd-i:] is a prefix of t[k:].
    for (auto &x : v) x = pd - x;
    // v[k] = the greatest pd - i < pd (and pd - i <= td-k) such that p[i:] is a prefix of t[k:].
    // i.e. v[k] = the least i > 0 (and pd - i <= td-k) such that p[i:] is a prefix of t[k:].
}

// Returns tpPTB[k] = is t[:k] a suffix of p, i.e. does it equal p[pd-k:]?
template<typename String>
void BuildPrefixTable2B(const String &t, int td, const String &p, int pd, const vector<int> &pPT, const vector<int> &tpPT, vector<bool> &tpPTB)
{
    tpPTB.resize(td + 1); for (int k = pd; k <= td; ++k) tpPTB[k] = false;
    // The longest proper suffix of p (i.e. shorter than p itself) that appears at the start of t is p[i:] for i = tpPT[0].
    // After that, the next shorter suffix of p that appears at the start of p[i:] (and thus at the start of t) is given by pPT[i].
    for (int k = tpPT[0]; k < pd; k = pPT[k]) 
        tpPTB[pd - k] = true;
    // Edge cases: t[:0] is an empty string and hence a suffix of p; t[:pd] is a suffix of p if it equals p,
    // which is if t[0] = p[0] and t[1:] starts with p[1:].
    tpPTB[0] = true; if (td >= pd && t[0] == p[0] && tpPT[1] == 1) tpPTB[pd] = true;
}

// tpSTB[k] = is t[k:] a prefix of p, i.e. does it equal p[:td-k]?
template<typename String>
void BuildSuffixTable2B(const String &t, int td, const String &p, int pd, const vector<int> &pST, const vector<int> &tpST, vector<bool> &tpSTB)
{
    tpSTB.resize(td + 1); for (int k = 0; k <= td - pd; ++k) tpSTB[k] = false;
    // The longest proper prefix of p (i.e. shorter than p itself) that appears at the end of t is p[:i] for i = tpST[td].
    // After that, the next shorter prefix of p that appears at the end of p[:i] (and thus at the end of t) is given by pST[i].
    for (int k = tpST[td]; k > 0; k = pST[k]) 
        tpSTB[td - k] = true;
    // Edge cases: t[td:] is an empty string and hence a prefix of p; t[td - pd:] is a prefix of p if it equals p,
    // which is if t[td-1] = p[pd-1] and t[:td-1] ends with with p[:pd-1].
    tpSTB[td] = true; if (td >= pd && t[td - 1] == p[pd - 1] && tpST[td - 1] == pd - 1) tpSTB[td - pd] = true;
}

struct Query { int i = -1, j = -1, k = -1, result = -1; };

struct Node
{
    int firstChild = -1, nextSib = -1;
    int startTime = -1, endTime = -1; // here the time is not incremented when exiting a node; thus the times to from 0 to nNodes - 1 and 'endTime' is the maximum of 'startTime' over all descendants of this node
};

void PreorderTraversal(const vector<int> &parent, vector<Node> &nodes)
{
    int n = parent.size(), root = -1; nodes.clear(); nodes.resize(n);
    // Use the 'parent' table to initialize the child and sibling links in 'nodes'.
    for (int u = 0; u < n; ++u) {
        int p = parent[u]; if (p < 0) { root = u; continue; }
        auto &U = nodes[u], &P = nodes[p];
        U.nextSib = P.firstChild; P.firstChild = u; }
    // Perform a preorder traversal.  The 'toDo' stack contains pairs (u, enter)
    // where 'enter' specifies if we are entering the subtree rooted by u or leaving it.
    stack<pair<int, bool>> toDo; toDo.emplace(root, true);
    int time = 0; // the current position in a preorder traversal
    while (! toDo.empty())
    {
        auto [u, enter] = toDo.top();  auto &U = nodes[u];
        // If we're leaving u's subtree, use time - 1 as the time of u's latest descendant.
        if (! enter) { U.endTime = time - 1; toDo.pop(); continue; }
        // If we're entering u's subtree, change the current pair at the top of the stack
        // from an 'enter' pair into an 'exit' pair, and add pairs telling the loop to
        // enter u's children.
        toDo.top().second = false; U.startTime = time++;
        for (int v = U.firstChild; v >= 0; v = nodes[v].nextSib) toDo.emplace(v, true);
    }
}

// Consider an array a[0..n-1]; let v[t] = sum a[f(t)+1...t], where f(t) is the value obtained by clearing the lowest bit that is set in t.
// This function updates v in a way that corresponds to increasing a[k] by 'delta'.
void FenwickUpdate(vector<int> &v, int k, int delta)
{
    if (k == 0) { v[k] += delta; return; }
    while (k < v.size()) { v[k] += delta; k += (k & ~(k - 1)); }
}

// Returns the sum of a[0..k].
int FenwickQuery(const vector<int> &v, int k)
{
    if (k < 0) return 0;
    int sum = v[0]; while (k > 0) { sum += v[k]; k &= k - 1; }
    return sum;
}

// For k = 0..sd, returns solutions[k] = the number of occurrences of p in s[:k] t s[k:].
void Solve(const string &s, const string &t, const string &p, vector<int> &solutions)
{
    const int sd = s.length(), td = t.length(), pd = p.length();
    // pST[k] = the greatest i < k such that p[:i] is a suffix of p[:k].
    vector<int> pST; BuildSuffixTable(p, pd, pST);
    // spST[k] = the greatest i < pd (and i <= k) such that p[:i] is a suffix of s[:k].
    vector<int> spST; BuildSuffixTable2(s, sd, p, pd, pST, spST);
    // occCountsS[k] = the number of occurrences of p in s[:k]
    vector<int> occCountsS; BuildOccCounts(s, sd, spST, p, pd, occCountsS);
    // pRST[k] = the greatest i < k such that p^R[:i] is a suffix of p^R[:k].
    vector<int> pRST; BuildSuffixTable(p.rbegin(), pd, pRST); 
    // pPT[k] = the least i > k such that p[i:] is a prefix of p[k:].
    vector<int> pPT; BuildPrefixTable(pRST, pPT);
    // tpRST[k] = the greatest i < pd (and i <= k) such that p^R[:i] is a suffix of t^R[:k].
    vector<int> tpRST; BuildSuffixTable2(t.rbegin(), td, p.rbegin(), pd, pRST, tpRST);
    // tpPT[k] = the least i > 0 such that p[i:] is a prefix of t[k:].
    vector<int> tpPT; BuildPrefixTable2(pd, tpRST, tpPT);
    // tpPTB[k] = is t[:k] a suffix of p, i.e. does it equal p[pd-k:]?
    vector<bool> tpPTB; BuildPrefixTable2B(t, td, p, pd, pPT, tpPT, tpPTB);
    // occCountsPT[k] = the number of occurrences of p in p[:k] t which straddle the boundary between p[:k] and t.
    vector<int> occCountsPT(pd + 1, 0); for (int k = 1; k < pd; ++k) occCountsPT[k] = occCountsPT[pST[k]] + (td >= pd - k && tpPTB[pd - k] ? 1 : 0);
    // spRST[k] = te greatest i < pd (and i <= k) such that p^R[:i] is a suffix of s^R[:k].
    vector<int> spRST; BuildSuffixTable2(s.rbegin(), sd, p.rbegin(), pd, pRST, spRST);
    // spPT[k] = the least i > 0 such that p[i:] is a prefix of s[k:].
    vector<int> spPT; BuildPrefixTable2(pd, spRST, spPT);
    // tpST[k] = the greatest i < pd (and i <= k) such that p[:i] is a suffix of t[:k].
    vector<int> tpST; BuildSuffixTable2(t, td, p, pd, pST, tpST);
    // tpSTB[k] = is t[k:] a prefix of p, i.e. does it equal p[:td-k]?
    vector<bool> tpSTB; BuildSuffixTable2B(t, td, p, pd, pST, tpST, tpSTB);
    // occCountsTP[k] = the number of occurrences of p in t p[k:] which straddle the boundary between t and p[k:].
    vector<int> occCountsTP(pd + 1, 0); for (int k = pd - 1; k > 0; --k) occCountsTP[k] = occCountsTP[pPT[k]] + (td >= k && tpSTB[td - k] ? 1 : 0);
    // occCountsT[k] = the number of occurrences of p in t[:k]
    vector<int> occCountsT; BuildOccCounts(t, td, tpST, p, pd, occCountsT);

    solutions.resize(sd + 1);
    for (int k = 0; k <= sd; ++k)
    {
        solutions[k] = occCountsT[td]; // occurrences of p that lie wholly in t
        solutions[k] += occCountsS[k]; // occurrences of p in s[:k]
        if (k + pd <= sd) solutions[k] += occCountsS[sd] - occCountsS[k + pd - 1]; // occurrences of p in s[k:]
        // Now let's count the occurrences of p in s[:k] t such that they
        // straddle the boundary between s[:k] and t.
        // This means that p can be split into a prefix p[:i] and suffix p[i:], where 0 < i < pd,
        // such that s[:k] ends with p[:i] and t starts with p[:i].
        // We know that the longest p[:i] that appears at the end of s[:k] is given by i = spST[k],
        // so instead of looking at s[:k] t we can look at p[:i] t instead.
        int i = spST[k]; solutions[k] += occCountsPT[i];
        // Similarly we can count the occurrences of p in t s[k:] that straddle
        // the boundary between t and s[k:].
        // This means that p can be split into a prefix p[:i] and suffix p[i:], where 0 < i < pd,
        // such that t ends with p[:i] and s[k:] starts with p[i:].
        // We know that the longest p[i:] that appears at the start of s[k:] is given by i = spPT[k],
        // so instead of looking at t s[k:] we can look at t p[i:] instead.
        i = spPT[k]; solutions[k] += occCountsTP[i];
    }
    if (td >= pd) return;
    // All that's left to count at this point are those occurrences of p in s[:k] t s[:k]
    // that stradle both the boundary between s[:k] and t, and the boundary between t and s[k:].

    // tST[k] = the greatest i < k such that t[:i] is a suffix of t[:k].
    vector<int> tST; BuildSuffixTable(t, td, tST);
    // ptST[k] = the greatest i < td (and i <= k) such that t[:i] is a suffix of p[:k].
    vector<int> ptST; BuildSuffixTable2(p, pd, t, td, tST, ptST);
    // tOccInP[k] = does t occur in p at position k, i.e. is p[k:k+td] = t?
    vector<bool> tOccInP(pd + 1, false); for (int k = 0; k + td <= pd; ++k) tOccInP[k] = (ptST[k + td - 1] == td - 1) && (p[k + td - 1] == t[td - 1]);
    // iNext[i] = the greatest i' < i such that p[:i] ends with p[:i'] and t occurs at position i' in p
    // jNext[j] = the greatest j' < j such that p[pd - j:] starts with p[pd - j':]
    vector<int> iNext(pd, 0), jNext(pd, 0); iNext[0] = -1; jNext[0] = -1;
    for (int i = 1; i < pd; ++i) if (int ii = pST[i]; ii > 0) iNext[i] = (tOccInP[ii]) ? ii : iNext[ii];
    for (int j = 1; j < pd; ++j) jNext[j] = pd - pPT[pd - j];
    vector<Node> iTree, jTree; PreorderTraversal(iNext, iTree); PreorderTraversal(jNext, jTree);

    // For p to straddle both boundaries, there must actually be something of s both to the left of t
    // and to the right of it -- in other words, k (the position where we insert t into s) must be > 0 and < sd.
    // For each such k, let p[:i_k] be longest prefix of p that appears at the end of s[:k],
    // and let p[j_k:] be the longest suffix of p that appears at the start of s[k:].
    // We will store i_k and j_k in queries[k-1].i and .j.
    vector<Query> queries(sd - 1);
    for (int k = 1; k < sd; ++k) { auto &Q = queries[k - 1]; Q.i = spST[k]; Q.j = spPT[k]; Q.k = k; Q.result = 0; }
    // Sort the queries in linear time.
    vector<int> firstQuery(pd, -1), nextQuery(queries.size(), -1);
    for (int qi = 0; qi < queries.size(); ++qi) { const auto &Q = queries[qi]; if (Q.i <= 0) continue;
        int x = iTree[Q.i].startTime; nextQuery[qi] = firstQuery[x]; firstQuery[x] = qi; }
    // Suppose that t occurs in p at position l, i.e. p[l:l+|t|] = t.  This accounts for one
    // occurrence of p in s[:k] t s[k:] for each k where s[:k] ends in p[:l] and s[k:] starts with p[l+|t|:].
    // This last condition is equivalent to saying that l must be an ancestor of i_k in iTree,
    // and l+|t| must be an ancestor of j_k in jTree.  For any given l, its descendants
    // in the first tree constitute a contiguous range in a preorder traversal of the tree,
    // and likewise the descendants of l+|t| in the second tree.  These two ranges can be thought of as
    // defining a rectangle R_l in the 2-dimensional plane, and our condition now becomes equivalent to
    // saying that (i_k, j_k) must lie in R_l.  Thus, for each k we must count the rectangles R_l that contain (i_k, j_k).
    // We will do this with a plane sweep along the x-axis.  The state of the rectangles along a vertical
    // line can be conveniently represented by a Fenwick tree: let a[y] be the difference between the
    // number of rectangles that start at this y-coordinate and the number of rectangles that end there.
    // Then let A[y] be the sum of a[0..y]; this is the number of rectangles that contain the point (x, y)
    // for the current value of y.
    struct Rectangle { int x1, x2, y1, y2; }; // {x1, ..., x2} \times {y1, ..., y2}.
    vector<Rectangle> rects;
    for (int l = 1; l + td < pd; ++l) if (tOccInP[l])
    {
        Rectangle R; const auto &Ni = iTree[l], &Nj = jTree[pd - (l + td)]; 
        R.x1 = Ni.startTime; R.x2 = Ni.endTime;
        R.y1 = Nj.startTime; R.y2 = Nj.endTime;
        rects.push_back(R);
    }
    // Sort the left and right edges of the rectangles in linear time.
    vector<int> firstLeft(pd, -1), firstRight(pd, -1), nextLeft(rects.size(), -1), nextRight(rects.size());
    for (int ri = 0; ri < rects.size(); ++ri) { auto &R = rects[ri]; 
        nextLeft[ri] = firstLeft[R.x1]; firstLeft[R.x1] = ri;
        nextRight[ri] = firstRight[R.x2]; firstRight[R.x2] = ri; }
    // Sweep the plane with a vertical line.  Note that the x-coordinate corresponds to a position
    // in a preorder traversal of iTree, and the y-coordinate to a position in a preorder traversal of jTree.
    vector<int> fenwickTree(pd + 1, 0);
    for (int x = 0; x < pd; ++x)
    {
        // Add the rectangles whose leftmost column is x.
        for (int ri = firstLeft[x]; ri >= 0; ri = nextLeft[ri]) {
            const auto &R = rects[ri]; FenwickUpdate(fenwickTree, R.y1, 1); FenwickUpdate(fenwickTree, R.y2 + 1, -1); }
        // Process the queries for this particular x.
        for (int qi = firstQuery[x]; qi >= 0; qi = nextQuery[qi]) {
            auto &Q = queries[qi]; if (Q.j >= pd) continue;
            int y = jTree[pd - Q.j].startTime; Q.result = FenwickQuery(fenwickTree, y); }
        // Remove the rectangles whose rightmost column is x.
        for (int ri = firstRight[x]; ri >= 0; ri = nextRight[ri]) {
            const auto &R = rects[ri]; FenwickUpdate(fenwickTree, R.y1, -1); FenwickUpdate(fenwickTree, R.y2 + 1, 1); }
    }
    for (auto &Q : queries) solutions[Q.k] += Q.result;
}

int main()
{
    // Read the input data.
    string s, t, p; getline(cin, s); getline(cin, t); getline(cin, p);
    // Find the number of occurrences of p in s[:k] t s[k:] for each k.
    vector<int> v; Solve(s, t, p, v);
    // Calculate the results that we have to output.
    int bestSol = -1, bestCount = -1, bestMin = -1, bestMax = -1;
    for (int k = 0; k <= s.size(); ++k) {
        if (v[k] > bestSol) bestSol = v[k], bestCount = 0, bestMin = k;
        if (v[k] == bestSol) ++bestCount, bestMax = k; }
    // Output the results.
    cout << bestSol << ' ' << bestCount << ' ' << bestMin << ' ' << bestMax << endl;
    return 0;
}
