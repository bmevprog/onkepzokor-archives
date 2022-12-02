// An O(s + t + sub p^2) solution that also works for |p| > |t|.
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
#include <chrono>
using namespace std;

#define Assert assert
//#define Assert(x) 

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct Timer
{
    typedef unsigned long long ull;
    ull tmStart;
    ull Now() const {
        FILETIME ct, et, kt, ut;
        GetProcessTimes(GetCurrentProcess(), &ct, &et, &kt, &ut);
        return ((ull(kt.dwHighDateTime) << 32) | kt.dwLowDateTime) + ((ull(ut.dwHighDateTime) << 32) | ut.dwLowDateTime); }
    explicit Timer(bool start = false) : tmStart(start ? Now() : 0) { }
    void Reset() { tmStart = Now(); }
    double Sec() const { return (Now() - tmStart) / 10000000.0; }
};

#else

struct Timer
{
    typedef std::chrono::time_point<std::chrono::steady_clock> T;
    T tmStart;
    T Now() const { return std::chrono::steady_clock::now(); }
    explicit Timer(bool start = false) { if (start) tmStart = Now(); }
    void Reset() { tmStart = Now(); }
    double Sec() const { return std::chrono::duration<double>(Now() - tmStart).count(); }
};

#endif

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

template<typename String>
void BuildSuffixTable_Naive(const String &p, int n, vector<int> &v)
{
    v.resize(n + 1);
    for (int k = 0; k <= n; ++k)
    {
        int i = k - 1;
        while (i > 0)
        {
            int j = 0; while (j < i && p[j] == p[k - i + j]) ++j;
            if (j >= i) break; else --i;
        }
        v[k] = i;
    }
}

template<typename String>
void TestBuildSuffixTable(const String &p, int n)
{
    vector<int> v1, v2; BuildSuffixTable(p, n, v1);
    BuildSuffixTable_Naive(p, n, v2);
    for (int k = 0; k <= n; ++k) Assert(v1[k] == v2[k]);
}

void TestBuildSuffixTable()
{
    for (int n = 0; n <= 20; ++n)
    {
        printf("%d\n", n); fflush(stdout);
        string s; s.resize(n);
        for (int i = 0; i < (1 << n); ++i)
        {
            for (int j = 0; j < n; ++j) s[j] = 'a' + ((i >> j) & 1);
            TestBuildSuffixTable(s, n);
        }
    }
}

// The following function computes, for the string s[0..sd-1], its suffix table with respect to p[0..pd-1],
// i.e. v[k] = the greatest i < pd (and i <= k) such that p[:i] is a suffix of s[:k].
// pST[k] must be the greatest i < k such that p[:i] is a suffix of p[:k].
template<typename String>
void BuildSuffixTable2(const String &s, int sd, const String &p, int pd, const vector<int> &pST, vector<int> &v)
{
    //if (s == "aa" && p == "aa") printf("!");
    Assert(pd > 0);
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

template<typename String>
void BuildSuffixTable2_Naive(const String &s, int sd, const String &p, int pd, vector<int> &v)
{
    //if (s == "aa" && p == "a") printf("!");
    v.resize(sd + 1);
    for (int k = 0; k <= sd; ++k)
    {
        int i = min(k, pd - 1);
        while (i > 0)
        {
            int j = 0; while (j < i && p[j] == s[k - i + j]) ++j;
            if (j >= i) break; else --i;
        }
        v[k] = i;        
    }
}

template<typename String>
void TestBuildSuffixTable2(const String &s, int sd, const String &p, int pd, const vector<int> &pST)
{
    vector<int> v1, v2; BuildSuffixTable2(s, sd, p, pd, pST, v1);
    BuildSuffixTable2_Naive(s, sd, p, pd, v2);
    for (int k = 0; k <= sd; ++k) Assert(v1[k] == v2[k]);
}

void TestBuildSuffixTable2()
{
    string s, p; vector<int> pST;
    for (int n = 0; n <= 12; ++n) for (int m = 1; m <= 6; ++m) 
    {
        printf("%d %d\n", n, m); fflush(stdout);
        s.resize(n); p.resize(m);
        for (int pi = 0; pi < (1 << m); ++pi)
        {
            for (int j = 0; j < m; ++j) p[j] = 'a' + ((pi >> j) & 1);
            BuildSuffixTable(p, m, pST);
            for (int si = 0; si < (1 << n); ++si)
            {
                for (int j = 0; j < n; ++j) s[j] = 'a' + ((si >> j) & 1);
                TestBuildSuffixTable2(s, n, p, m, pST);
            }
        }
    }
}

// For k = 0..sd, returns v[k] = number of occurrences of p in s[:k].
// spST[k] must be the greatest i < min(pd, k) such that p[:i] is a suffix of s[:k].
template<typename String>
void BuildOccCounts(const String &s, int sd, const vector<int> &spST, const String &p, int pd, vector<int> &v)
{
    //if (s == "aa" && p == "a") printf("!");
    v.resize(sd + 1); v[0] = 0;
    const auto pLast = p[pd - 1];
    for (int k = 1; k <= sd; ++k)
        v[k] = v[k - 1] + (spST[k - 1] == pd - 1 && s[k - 1] == pLast ? 1 : 0);
}

template<typename String>
void BuildOccCounts_Naive(const String &s, int sd, const String &p, int pd, vector<int> &v)
{
    v.resize(sd + 1);
    int count = 0;
    for (int k = 0; k <= sd; ++k)
    {
        int i = 0; while (i < k && i < pd && s[k - 1 - i] == p[pd - 1 - i]) ++i;
        if (i == pd) ++count;
        v[k] = count;
    }
}

template<typename String>
void TestBuildOccCounts(const String &s, int sd, const vector<int> &spST, const String &p, int pd)
{
    vector<int> v1, v2; BuildOccCounts(s, sd, spST, p, pd, v1);
    BuildOccCounts_Naive(s, sd, p, pd, v2);
    for (int k = 0; k <= sd; ++k) Assert(v1[k] == v2[k]);
}

void TestBuildOccCounts()
{
    string s, p; vector<int> pST, spST;
    for (int n = 0; n <= 12; ++n) for (int m = 1; m <= 6; ++m) 
    {
        printf("%d %d\n", n, m); fflush(stdout);
        s.resize(n); p.resize(m);
        for (int pi = 0; pi < (1 << m); ++pi)
        {
            for (int j = 0; j < m; ++j) p[j] = 'a' + ((pi >> j) & 1);
            BuildSuffixTable(p, m, pST);
            for (int si = 0; si < (1 << n); ++si)
            {
                for (int j = 0; j < n; ++j) s[j] = 'a' + ((si >> j) & 1);
                BuildSuffixTable2(s, n, p, m, pST, spST);
                TestBuildOccCounts(s, n, spST, p, m);
            }
        }
    }
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

template<typename String>
void BuildPrefixTable_Naive(const String &p, int n, vector<int> &v)
{
    v.resize(n + 1);
    for (int k = 0; k <= n; ++k)
    {
        int i = k + 1;
        while (i < n)
        {
            int j = i; while (j < n && p[j] == p[k - i + j]) ++j;
            if (j >= n) break; else ++i;
        }
        v[k] = i;
    }
}

template<typename String>
void TestBuildPrefixTable(const String &p, int n)
{
    vector<int> pRST; BuildSuffixTable(p.rbegin(), n, pRST); 
    vector<int> v1, v2; BuildPrefixTable(pRST, v1);
    BuildPrefixTable_Naive(p, n, v2);
    for (int k = 0; k <= n; ++k) Assert(v1[k] == v2[k]);
}

void TestBuildPrefixTable()
{
    for (int n = 0; n <= 20; ++n)
    {
        printf("%d\n", n); fflush(stdout);
        string s; s.resize(n);
        for (int i = 0; i < (1 << n); ++i)
        {
            for (int j = 0; j < n; ++j) s[j] = 'a' + ((i >> j) & 1);
            TestBuildPrefixTable(s, n);
        }
    }
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

template<typename String>
void BuildPrefixTable2_Naive(const String &t, int td, const String &p, int pd, vector<int> &v)
{
    v.resize(td + 1);
    for (int k = 0; k <= td; ++k)
    {
        int i = 1;
        while (i < pd)
        {
            if (pd - i > td - k) { ++i; continue; }
            int j = i; while (j < pd && t[k + j - i] == p[j]) ++j;
            if (j >= pd) break; else ++i;
        }
        v[k] = i;
    }
}

template<typename String>
void TestBuildPrefixTable2(const String &t, int td, const String &p, int pd, const vector<int> &pRST)
{
    vector<int> tpRST; BuildSuffixTable2(t.rbegin(), td, p.rbegin(), pd, pRST, tpRST);
    vector<int> v1, v2; BuildPrefixTable2(pd, tpRST, v1);
    BuildPrefixTable2_Naive(t, td, p, pd, v2);
    //if (t == "" && p == "a") printf("!");
    for (int k = 0; k <= td; ++k) Assert(v1[k] == v2[k]);
}

void TestBuildPrefixTable2()
{
    string s, p; vector<int> pRST;
    for (int n = 0; n <= 12; ++n) for (int m = 1; m <= 6; ++m) 
    {
        printf("%d %d\n", n, m); fflush(stdout);
        s.resize(n); p.resize(m);
        for (int pi = 0; pi < (1 << m); ++pi)
        {
            for (int j = 0; j < m; ++j) p[j] = 'a' + ((pi >> j) & 1);
            vector<int> pRST; BuildSuffixTable(p.rbegin(), m, pRST); 
            for (int si = 0; si < (1 << n); ++si)
            {
                for (int j = 0; j < n; ++j) s[j] = 'a' + ((si >> j) & 1);
                TestBuildPrefixTable2(s, n, p, m, pRST);
            }
        }
    }
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

template<typename String>
void BuildPrefixTable2B_Naive(const String &t, int td, const String &p, int pd, vector<bool> &tpPTB)
{
    tpPTB.resize(td + 1);
    for (int k = 0; k <= td; ++k)
    {
        if (k > pd) { tpPTB[k] = false; continue; }
        int i = 0; while (i < k && t[i] == p[pd - k + i]) ++i;
        tpPTB[k] = (i == k);
    }
}

template<typename String>
void TestBuildPrefixTable2B(const String &t, int td, const String &p, int pd, const vector<int> &pRST, const vector<int> &pPT)
{
    vector<int> temp; BuildSuffixTable(p.rbegin(), pd, temp);  Assert(temp == pRST);
    BuildPrefixTable(pRST, temp); Assert(temp == pPT);
    vector<int> tpRST; BuildSuffixTable2(t.rbegin(), td, p.rbegin(), pd, pRST, tpRST);
    vector<int> tpPT; BuildPrefixTable2(pd, tpRST, tpPT);
    BuildPrefixTable2_Naive(t, td, p, pd, temp); Assert(temp == tpPT);
    vector<bool> v1, v2; BuildPrefixTable2B(t, td, p, pd, pPT, tpPT, v1);
    BuildPrefixTable2B_Naive(t, td, p, pd, v2);
    for (int k = 0; k <= td; ++k) Assert(v1[k] == v2[k]);
}

void TestBuildPrefixTable2B()
{
    string s, p; vector<int> pRST;
    for (int n = 0; n <= 12; ++n) for (int m = 1; m <= 6; ++m) 
    {
        printf("%d %d\n", n, m); fflush(stdout);
        s.resize(n); p.resize(m);
        for (int pi = 0; pi < (1 << m); ++pi)
        {
            for (int j = 0; j < m; ++j) p[j] = 'a' + ((pi >> j) & 1);
            vector<int> pRST; BuildSuffixTable(p.rbegin(), m, pRST); 
            vector<int> pPT; BuildPrefixTable(pRST, pPT);
            for (int si = 0; si < (1 << n); ++si)
            {
                for (int j = 0; j < n; ++j) s[j] = 'a' + ((si >> j) & 1);
                TestBuildPrefixTable2B(s, n, p, m, pRST, pPT);
            }
        }
    }
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

template<typename String>
void BuildSuffixTable2B_Naive(const String &t, int td, const String &p, int pd, vector<bool> &tpSTB)
{
    tpSTB.resize(td + 1);
    for (int k = 0; k <= td; ++k)
    {
        if (td - k > pd) { tpSTB[k] = false; continue; }
        int i = k; while (i < td && t[i] == p[i - k]) ++i;
        tpSTB[k] = (i == td);
    }
}

template<typename String>
void TestBuildSuffixTable2B(const String &t, int td, const String &p, int pd, const vector<int> &pST)
{
    vector<int> tpST; BuildSuffixTable2(t, td, p, pd, pST, tpST);
    vector<bool> v1, v2; BuildSuffixTable2B(t, td, p, pd, pST, tpST, v1);
    BuildSuffixTable2B_Naive(t, td, p, pd, v2);
    for (int k = 0; k <= td; ++k) Assert(v1[k] == v2[k]);
}

void TestBuildSuffixTable2B()
{
    string s, p; vector<int> pRST;
    for (int n = 0; n <= 12; ++n) for (int m = 1; m <= 6; ++m) 
    {
        printf("%d %d\n", n, m); fflush(stdout);
        s.resize(n); p.resize(m);
        for (int pi = 0; pi < (1 << m); ++pi)
        {
            for (int j = 0; j < m; ++j) p[j] = 'a' + ((pi >> j) & 1);
            vector<int> pST; BuildSuffixTable(p, m, pST);
            for (int si = 0; si < (1 << n); ++si)
            {
                for (int j = 0; j < n; ++j) s[j] = 'a' + ((si >> j) & 1);
                TestBuildSuffixTable2B(s, n, p, m, pST);
            }
        }
    }
}

struct Query
{
    int i = -1, j = -1, k = -1, result = -1, temp;
};

struct Node
{
    int parent = -1, firstChild = -1, nextSib = -1;
    int startTime = -1, endTime = -1; // indexes into 'dftOrder'
    int height = -1; // distance from the deepest descendant
    int depth = -1; // distance from the root
    int nDesc = 0; // number of descendants, including this node itself
    // Similar to how firstChild and nextSib are used to maintain a linked
    // list of children, we will use firstPartDesc and nextPartSib to maintain
    // a linked list of the roots of subtrees obtained during partitioning.
    // Note that the current node itself is in a sense a part-root as well (of
    // whatever is left after detaching the subtrees rooted by the other part-roots),
    // but it is not included in this linked list.
//    int firstPartDesc = -1, nextPartSib = -1;
    int nUnpartDesc; // number of descendants that have not yet been cut off during the partitioning
    int detached = -1; // the level of partitioning at which this node was detached from its parent
};
struct PartRoot
{
    int node = -1; // the corresponding node in 'nodes'
    int parent = -1, level = -1, firstChild = -1, nextSib = -1; // indices into 'partRoots'
    int nDesc = 0, height = 0;
    PartRoot(int Node, int NDesc) : node(Node), nDesc(NDesc) { }
};

constexpr bool DEBUG = true;

struct Tree
{
    vector<Node> nodes; int root = -1;
    vector<PartRoot> partRoots; int partRoot = -1;
    vector<vector<int>> partitionMap; // partitionMap[l][u] = the part-root of the tree containing node u at the l'th level of partitioning
//    vector<pair<int, bool>> dftOrder; // (node, isEnter) pairs
    void DepthFirstTraversal()
    {
        stack<pair<int, bool>> toDo; toDo.emplace(root, true);
        int time = 0; //dftOrder.clear(); dftOrder.reserve(nodes.size());
        while (! toDo.empty())
        {
            auto [u, enter] = toDo.top(); //dftOrder.push_back(toDo.top());
            auto &U = nodes[u];
            Node *P = (U.parent >= 0) ? &nodes[U.parent] : nullptr;
            if (! enter) { 
                U.endTime = time++; toDo.pop(); 
                if (P) { P->height = max(P->height, U.height + 1); P->nDesc += U.nDesc; }
                continue; }
            toDo.top().second = false; U.startTime = time++;
            U.depth = (P ? P->depth + 1 : 0);
            U.height = 0; U.nDesc = 1;
            for (int v = U.firstChild; v >= 0; v = nodes[v].nextSib) toDo.emplace(v, true);
        }
    }
    Tree() = default;
    explicit Tree(const vector<int> &parent) : nodes(parent.size())
    {
        for (int u = 0, n = parent.size(); u < n; ++u) {
            int p = parent[u]; if (p < 0) { Assert(root < 0); root = u; continue; }
            auto &U = nodes[u], &P = nodes[p];
            U.parent = p; U.nextSib = P.firstChild; P.firstChild = u; }
        DepthFirstTraversal();
    }
    // Returns 'true' if 'anc' is an ancestor of 'desc' (or the asme node).
    bool IsAncestorOf(int anc, int desc) const 
    {
        const auto &D = nodes[desc], &A = nodes[anc];
        return A.startTime <= D.startTime && D.endTime <= A.endTime; 
    }
    // Proceed from the bottom upwards and cut off any subtrees with >= 'maxSubtreeSize' nodes.
    // The roots of such trees are considered to be 'marked', as is the root of the entire tree.
    // The vector 'closestMarkedAncestor[u]' receives the pair (p, d) where p is the closest marked
    // ancestor of u, and d is the distance between p and u.
    int MarkNodes(int maxSubtreeSize, vector<pair<int, int>> &closestMarkedAncestor)
    {
        int n = nodes.size(), nMarked = 0; closestMarkedAncestor.resize(n); for (auto &x : closestMarkedAncestor) x = {-1, -1};
        vector<int> subtreeSize(n, 0), preOrderTraversal;
        stack<pair<int, bool>> toDo; toDo.emplace(root, true);
        while (! toDo.empty())
        {
            auto [u, enter] = toDo.top(); auto &U = nodes[u];
            if (! enter) { 
                if (subtreeSize[u] >= maxSubtreeSize || u == root) closestMarkedAncestor[u] = {u, 0}, ++nMarked;
                else if (int p = U.parent; p >= 0) subtreeSize[p] += subtreeSize[u]; 
                toDo.pop(); continue; }
            preOrderTraversal.emplace_back(u); 
            toDo.top().second = false; subtreeSize[u] = 1;
            for (int v = U.firstChild; v >= 0; v = nodes[v].nextSib) toDo.emplace(v, true);
        }
        for (int u : preOrderTraversal) { auto &C = closestMarkedAncestor[u]; if (C.second >= 0) continue;
            auto &P = closestMarkedAncestor[nodes[u].parent];
            C = {P.first, P.second + 1}; }
        if constexpr (DEBUG) if (n > 1000) fprintf(stderr, "MarkNodes: maxSize = %d -> %d/%d nodes marked\n", maxSubtreeSize, nMarked, n); 
        return nMarked;
    }
   
protected:
    // Partitions the subtree rooted by 'u', minus whatever has already been detached from it earlier.
    // Returns the indes of the corresponding new part-root which it creates in 'partRoots'.
    int PartitionTree(int u, int level)
    {
        auto &U = nodes[u]; 
        int ur = partRoots.size(); partRoots.emplace_back(u, U.nUnpartDesc);
        auto *UR = &partRoots[ur]; UR->height = 0; UR->level = level;
        // Prepare a list of u's descendants, in depth-first order.
        // We must of course skip any descendants that have been detached at higher levels of partitioning.
        vector<int> uDesc; 
        stack<pair<int, bool>> toDo; toDo.emplace(u, true);
        while (! toDo.empty()) {
            auto [v, enter] = toDo.top();
            if (! enter) { uDesc.emplace_back(v); toDo.pop(); continue; }
            toDo.top().second = false;
            for (int w = nodes[v].firstChild; w >= 0; w = nodes[w].nextSib)
                if (nodes[w].detached < 0) toDo.emplace(w, true); }
        Assert(uDesc.size() == U.nUnpartDesc); // later this will no longer hold when we detach smaller subtrees from u's subtree
        // Mark the descendants as belonging to this part-root; also calculate the height of UR's tree.
        while (partitionMap.size() <= level) partitionMap.emplace_back(nodes.size(), -1);
        for (int w : uDesc) {
            partitionMap[level][w] = ur;
            UR->height = max(UR->height, nodes[w].depth - U.depth); }
        if (U.nUnpartDesc <= 1) return ur; // if u is a leaf, there's nothing to partition
        if (UR->height <= 2) return ur; // we won't partition trees that are shallow enough
        // Try to find a suitable q to partition the subtree.
        for (int k = 1 ; ; ++k)
        {
            int q = (uDesc.size() + (1 << k) - 1) >> k; // = ceil(|T(u)| / 2^k)
            // We'll try to partition T(u) [the subtree rooted by u]
            // by cutting, from the bottom up, subtrees of at least q elements.
            // More precisely: if w is a descendant of u where |w.nUncutDesc| >= q
            // and all its (not-yet-cut) children c have |c.nUncutDesc| < q, then we will cut T(w)
            // out of the tree, and create a corresponding part-root which becomes a subroot of 'ur'.
            // We repeat this until T(u) is completely gone; at the last step, u itself is the 
            // last node to be cut and a new part-root added as a subroot of 'ur'.
            // However, it is possible that u is also the only node to be cut thusly, 
            // in which case our "partition" is trivial and we'll have to try again with a smaller q.
            int nSubRoots = 0;
            for (int w : uDesc)
            {
                auto &W = nodes[w]; Assert(W.nUnpartDesc > 0);
                // If we reach u without having detached anything below it, we'll have to try again with a smaller q.
                if (w == u && nSubRoots == 0) break;
                // Perhaps w's subtree is not big enough to detach.
                if (w != u && W.nUnpartDesc < q) continue;
                // Detach it.
                if constexpr (false) printf("Level %d, u = %d (ur = %d; %d unpartDesc); k = %d (q = %d); detaching w = %d; w.nUnpartDesc = %d\n", level, u, ur, int(uDesc.size()), k, q, w, W.nUnpartDesc);
                W.detached = level; ++nSubRoots;
                if (w != u) {
                    int delta = W.nUnpartDesc, z = W.parent;
                    while (true) { auto &Z = nodes[z]; Z.nUnpartDesc -= delta; if (z == u) break; else z = Z.parent; } }
                // T(w) has at least q nodes; thus, if w were more than q levels below u, then the
                // path from u to w would contain more than q nodes, and together with T(w) this would
                // mean more than 2q nodes; now if k = 1, this would be more than U.nDesc, which is impossible;
                // and if k > 1, then we should have been able to find a nontrivial partition at k-1
                // by using one of the nodes on the path between u and w as a part-root.
                Assert(W.depth <= U.depth + q);
                int wr = PartitionTree(w, level + 1);
                UR = &partRoots[ur]; // the vector may have been reallocated, so we need a new pointer
                partRoots[wr].nextSib = UR->firstChild; UR->firstChild = wr;
                partRoots[wr].parent = ur;
            }
            if (nSubRoots == 0) { Assert(q > 1); continue; }
            Assert(nSubRoots >= 2); 
            Assert(nSubRoots <= (1 << k));
            break;
        }
        return ur;
    }
public:
    void PartitionTree() 
    { 
        partitionMap.clear(); partRoots.clear();
        for (auto &U : nodes) { U.nUnpartDesc = U.nDesc; U.detached = -1; }
        partRoot = PartitionTree(root, 0); 
    }
};

void TestPartitionTree(const vector<int> parent)
{
    Tree tree(parent);
    tree.PartitionTree();
    printf("Partitioned %d into %d levels, %d roots.\n", int(tree.nodes.size()), int(tree.partitionMap.size()), int(tree.partRoots.size()));
}

void TestPartitionTree(int n)
{
    vector<int> parent(n, -1);
    for (int u = 1; u < n; ++u) parent[u] = u - 1;
    TestPartitionTree(parent);
    for (int u = 1; u < n; ++u) parent[u] = 0;
    TestPartitionTree(parent);
    for (int nIter = 0; nIter < 6; ++nIter)
    {
        mt19937_64 r(123 + nIter);
        for (int u = 1; u < n; ++u) parent[u] = uniform_int_distribution<int>(0, u - 1)(r);
        TestPartitionTree(parent);
    }
}

void TestPartitionTree()
{
    TestPartitionTree(10);
    TestPartitionTree(100);
    TestPartitionTree(1000);
    TestPartitionTree(10000);
    TestPartitionTree(100000);
}

struct QueryProcessor
{
    vector<Query> queries;
    Tree &iTree, &jTree;
    vector<bool> &tOccInP; int td, pd;
    vector<int> *f; // for debugging only
    int opCount, opCountRec, opCountS;
    QueryProcessor(Tree &iTree_, Tree &jTree_, vector<bool> &tOccInP_, int td_, int pd_, vector<int> *f_) : iTree(iTree_), jTree(jTree_), tOccInP(tOccInP_), td(td_), pd(pd_), f(f_) { }
protected:
    // Calculates f(i2, j) from f(i1, j), assuming that i2 is a descendant of i1.
    int CalcByI(int i1, int j, int fi1j, int i2)
    {
        if constexpr (DEBUG) if (f) Assert((*f)[i1 * pd + j] == fi1j);
        Assert(iTree.IsAncestorOf(i1, i2));
        int d1 = iTree.nodes[i1].depth, d2 = iTree.nodes[i2].depth;
        static vector<pair<int, int>> w; if (w.size() <= d2 - d1 + 1) w.resize(d2 - d1 + 1);
        // Prepare a list of ancestors from i1 to i2.
        w[d2 - d1].first = i2;
        for (int d = d2 - 1, u = i2; d >= d1; --d) { u = iTree.nodes[u].parent; w[d - d1].first = u; }
        Assert(w[0].first == i1);
        w[0].second = fi1j;
        // Calculate f.
        for (int d = d1 + 1; d <= d2; ++d)
        {
            int parent = w[d - 1 - d1].first, fParent = w[d - 1 - d1].second, child = w[d - d1].first;
            // Does p occur at the start of p[:child] t p[pd - j:]?
            int F = 0;
            if (int right = pd - child - td; right > 0 && right <= j && tOccInP[child]) 
                // Now we have to check if p[pd - j:] starts with p[i':] for i' = child + td,
                // which is the same as p[pd - right:].
                if (jTree.IsAncestorOf(right, j)) ++F;
            // Next, count the occurrences of p later in the string.
            F += fParent; // f(parent, j)
            w[d - d1].second = F;
            if constexpr (DEBUG) if (f) Assert((*f)[child * pd + j] == F);
        }
        opCount += d2 - d1;
        return w[d2 - d1].second;
    }
    // Calculates f(i, j2) from f(i, j1), assuming that j2 is a descendant of j1.
    int CalcByJ(int i, int j1, int fij1, int j2)
    {
        if constexpr (DEBUG) if (f) Assert((*f)[i * pd + j1] == fij1);
        Assert(jTree.IsAncestorOf(j1, j2));
        int d1 = jTree.nodes[j1].depth, d2 = jTree.nodes[j2].depth;
        static vector<pair<int, int>> w; if (w.size() <= d2 - d1 + 1) w.resize(d2 - d1 + 1);
        // Prepare a list of ancestors from j1 to j2.
        w[d2 - d1].first = j2;
        for (int d = d2 - 1, u = j2; d >= d1; --d) { u = jTree.nodes[u].parent; w[d - d1].first = u; }
        Assert(w[0].first == j1);
        w[0].second = fij1;
        // Calculate f.
        for (int d = d1 + 1; d <= d2; ++d)
        {
            int parent = w[d - 1 - d1].first, fParent = w[d - 1 - d1].second, child = w[d - d1].first;
            // Does p occur at the end of p[:i] t p[pd - child:]?
            int F = 0;
            if (int left = pd - child - td; left > 0 && left <= i && tOccInP[left])
                // Now we have to check if p[:i] ends with p[:left].
                if (iTree.IsAncestorOf(left, i)) ++F;
            // Next, count the occurrences of p earlier in the string.
            F += fParent; // f(i, parent)
            w[d - d1].second = F;
            if constexpr (DEBUG) if (f) Assert((*f)[i * pd + child] == F);
        }
        opCount += d2 - d1;
        return w[d2 - d1].second;
    }
    void Process(int ir, int jr, int fij, int qFrom, int qToBefore)
    {
        auto &IR = iTree.partRoots[ir], &JR = jTree.partRoots[jr];
        const int iLevel = IR.level, jLevel = JR.level, iNode = IR.node, jNode = JR.node;
        // fij is assumed to contain the value of f(IR.node, JR.node);
        // and all the queries in [qFrom..qToBefore) refer to values in their descendants.
        if constexpr (DEBUG) for (int q = qFrom; q < qToBefore; ++q) {
            auto &Q = queries[q];
            Assert(iTree.partitionMap[iLevel][Q.i] == ir);
            Assert(jTree.partitionMap[jLevel][Q.j] == jr);
            Assert(iTree.IsAncestorOf(iNode, Q.i));
            Assert(jTree.IsAncestorOf(jNode, Q.j)); }
        if (IR.firstChild < 0 && JR.firstChild < 0)
        {
            // Answer the queries; we'll group them up by i to save a little time.
            sort(queries.begin() + qFrom, queries.begin() + qToBefore, [] (const auto &x, const auto &y) { return (x.i < y.i) || (x.i == y.i && x.j < y.j); });
            int ii = -1, fiij = -1;
            vector<pair<int, int>> v; v.resize(IR.height + 1);
            opCount = 0; 
            for (int q = qFrom; q < qToBefore; ++q) 
            {
                auto &Q = queries[q];
                if (Q.i != ii) { ii = Q.i; fiij = CalcByI(iNode, jNode, fij, ii); }
                Q.result = CalcByJ(ii, jNode, fiij, Q.j);
            }
            opCountS += opCount;
        }
        else if (IR.height > JR.height)
        {
            for (int q = qFrom; q < qToBefore; ++q) { auto &Q = queries[q]; Q.temp = iTree.partitionMap[iLevel + 1][Q.i]; }
            sort(queries.begin() + qFrom, queries.begin() + qToBefore, [] (const auto &x, const auto &y) { return x.temp < y.temp; });
            int iir = -1, fiij = -1;
            for (int q = qFrom; q < qToBefore; )
            {
                auto &Q = queries[q]; 
                iir = Q.temp; opCount = 0; fiij = CalcByI(iNode, jNode, fij, iTree.partRoots[iir].node); opCountRec += opCount;
                int qq = q + 1; while (qq < qToBefore && queries[qq].temp == iir) ++qq;
                Process(iir, jr, fiij, q, qq); q = qq;
            }
        }
        else
        {
            for (int q = qFrom; q < qToBefore; ++q) { auto &Q = queries[q]; Q.temp = jTree.partitionMap[jLevel + 1][Q.j]; }
            sort(queries.begin() + qFrom, queries.begin() + qToBefore, [] (const auto &x, const auto &y) { return x.temp < y.temp; });
            int jjr = -1, fijj = -1;
            for (int q = qFrom; q < qToBefore; )
            {
                auto &Q = queries[q]; 
                jjr = Q.temp; opCount = 0; fijj = CalcByJ(iNode, jNode, fij, jTree.partRoots[jjr].node); opCountRec += opCount;
                int qq = q + 1; while (qq < qToBefore && queries[qq].temp == jjr) ++qq;
                Process(ir, jjr, fijj, q, qq); q = qq;
            }
        }
    }
public:
    void Process() { opCount = 0; opCountRec = 0; opCountS = 0; Process(iTree.partRoot, jTree.partRoot, 0, 0, queries.size()); }
};

struct Solution 
{ 
    // This is the actual solution.
    int bestSol = -1, bestCount = -1, bestMin = -1, bestMax = -1; 
    // The rest is profiling information from Solve() and its subordinate calls.
    int sd = -1, td = -1, pd = -1;
    double tmLinPart = 0, tmQuadPart = 0, tmSubQuadPart = 0;
    int nDifQueries = 0, qiMax = 0, qjMax = 0, opCountRec = 0, opCountS = 0;

    Solution &Init(const vector<int> &sol)
    {
        bestSol = -1; bestCount = 0; bestMin = -1; bestMax = -1;
        for (int k = 0; k < sol.size(); ++k) {
            if (sol[k] > bestSol) bestSol = sol[k], bestCount = 0, bestMin = k;
            if (sol[k] == bestSol) ++bestCount, bestMax = k; }
        return *this;
    }
    Solution() = default;
    explicit Solution(const vector<int> &sol) { Init(sol); }

    double Interestingness() const
    {
        // Higher is better.
        int score1 = (bestSol == 0) ? 1 : (bestSol == 1) ? 2 : 3;
        int score2 = (bestCount == 0) ? 1 : (bestCount == 1 || bestCount >= sd - 10) ? 2 : (bestCount < 10) ? 3 : 4;
        int score3 = (bestMin == 0) ? 1 : (bestMin == 1) ? 2 : 10;
        int score4 = (bestMax == sd) ? 1 : (bestMin == sd - 1) ? 2 : 10;
        double score5 = double(qiMax + 1) * (qjMax + 1) / double(pd) / pd + 0.2;
        double score6 = (1 + nDifQueries) / double(sd + 1) + 0.2;
        // Lower is better.
        double score = 1.0 / double(score1) + 1.0 / double(score2) + 1.0 / double(score3) + 1.0 / double(score4) + 1.0 / score5 + 1.0 / score6;
        // Higher is better.
        return 6.0 / score;
    }
};


// For k = 0..sd, returns solutions[k] = the number of occurrences of p in s[:k] t s[k:].
Solution Solve(const string &s, const string &t, const string &p, vector<int> &solutions)
{
    const int sd = s.length(), td = t.length(), pd = p.length();
    const bool timing = DEBUG && (sd >= 30 || td >= 30 || pd >= 30);
    Timer tm(timing); Solution sol; sol.sd = sd; sol.td = td; sol.pd = pd;
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
        // straddle the boundary between s[:k] and t.  This means that p can be 
        // split into a prefix p[:i] and suffix p[i:], where 0 < i < pd,
        // such that s[:k] ends with p[:i] and t starts with p[:i].
        // We know that the longest p[:i] that appears at the end of s[:k] is given by i = spST[k],
        // so instead of looking at s[:k] t we can look at p[:i] t instead.
        int i = spST[k]; solutions[k] += occCountsPT[i];
        // Similarly we can count the occurrences of p in t s[k:] that straddle
        // the boundary between t and s[k:].  This means that p can be split into
        // a prefix p[:i] and suffix p[i:], where 0 < i < pd,
        // such that t ends with p[:i] and s[k:] starts with p[i:].
        // We know that the longest p[i:] that appears at the start of s[k:] is given by i = spPT[k],
        // so instead of looking at t s[k:] we can look at t p[i:] instead.
        i = spPT[k]; solutions[k] += occCountsTP[i];
    }
    sol.tmLinPart = timing ? tm.Sec() : 0; sol.tmQuadPart = 0; sol.tmSubQuadPart = 0;
    if (td >= pd) return sol.Init(solutions);

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
    Tree iTree(iNext), jTree(jNext); Assert(iTree.root == 0); Assert(jTree.root == 0);

    vector<int> f, *fPtr = nullptr;

    if constexpr (DEBUG) if (sd <= 1000 && td <= 1000 && pd <= 1000)
    {
        fPtr = &f;
        if (timing) tm.Reset();
        // f[i * pd + j] = the number of occurrences of p in p[:i] t p[pd - j:] which straddle both boundaries.
        f.clear(); f.resize(pd * pd, 0); 
        for (int i = 1; i < pd; ++i) for (int j = 1; j < pd; ++j) if (i + j + td >= pd)
        {
            // Does p occur at the start of p[:i] t p[pd - j:]?
            int F = 0;
            if (int right = pd - i - td; right > 0 && right <= j && tOccInP[i]) 
                // Now we have to check if p[pd - j:] starts with p[i':] for i' = i + td,
                // which is the same as p[pd - right:].
                if (jTree.IsAncestorOf(right, j)) ++F;
            // Next, count the occurrences of p later in the string.
            if (int ii = iNext[i]; ii > 0) F += f[ii * pd + j];
            if constexpr (true)
            {
                // Alternatively, we can start by checking if p occurs at the end of p[:i] t p[pd - j:].
                int F2 = 0;
                if (int left = pd - j - td; left > 0 && left <= i && tOccInP[left])
                    // Now we have to check if p[:i] ends with p[:left].
                    if (iTree.IsAncestorOf(left, i)) ++F2;
                // Next, count the occurrences of p earlier in the string.
                if (int jj = jNext[j]; jj > 0) F2 += f[i * pd + jj];
                Assert(F2 == F);
            }
            f[i * pd + j] = F;
        }
        if (timing) sol.tmQuadPart = tm.Sec();
    }

    if constexpr (false)
    {
        if (timing) tm.Reset();
        // A fairly naive solution that calculates each f(i, j) from the previous one.  Too slow.
        long long int nOpsI = 0, nOpsJ = 0;
        int i = 0, j = 0, fij = 0; vector<int> ancPath(pd);
        for (int k = 1; k < sd; ++k)
        {
            int qi = spST[k], qj = pd - spPT[k]; 
            if constexpr (false) { i = 0; j = 0; fij = 0; } // or we could start from (0, 0) each time...
            // Go up the first tree.
            while (! iTree.IsAncestorOf(i, qi))
            {
                int F = fij; int ii = iNext[i];
                if (int right = pd - i - td; right > 0 && right <= j && tOccInP[i])
                    if (jTree.IsAncestorOf(right, j)) --F;
                if (fPtr) Assert((*fPtr)[ii * pd + j] == F);
                i = ii; fij = F; ++nOpsI;
            }
            if (fPtr) Assert((*fPtr)[i * pd + j] == fij);
            // Go down the first tree.
            int step = 0, anc = qi; while (anc != i) { ancPath[step++] = anc; anc = iNext[anc]; }
            while (step-- > 0)
            {
                int ii = ancPath[step]; Assert(iNext[ii] == i);
                int F = fij;
                if (int right = pd - ii - td; right > 0 && right <= j && tOccInP[ii])
                    if (jTree.IsAncestorOf(right, j)) ++F;
                if (fPtr) Assert((*fPtr)[ii * pd + j] == F);
                i = ii; fij = F; ++nOpsI;
            }
            Assert(i == qi); if (fPtr) Assert((*fPtr)[i * pd + j] == fij);
            // Go up the second tree.
            while (! jTree.IsAncestorOf(j, qj))
            {
                int F = fij; int jj = jNext[j];
                if (int left = pd - j - td; left > 0 && left <= i && tOccInP[left])
                    if (iTree.IsAncestorOf(left, i)) --F;
                if (fPtr) Assert((*fPtr)[i * pd + jj] == F);
                j = jj; fij = F; ++nOpsJ;
            }
            // Go down the second tree.
            step = 0; anc = qj; while (anc != j) { ancPath[step++] = anc; anc = jNext[anc]; }
            while (step-- > 0)
            {
                int jj = ancPath[step]; Assert(jNext[jj] == j);
                int F = fij;
                if (int left = pd - jj - td; left > 0 && left <= i && tOccInP[left])
                    if (iTree.IsAncestorOf(left, i)) ++F;
                if (fPtr) Assert((*fPtr)[i * pd + jj] == F);
                j = jj; fij = F; ++nOpsJ;
            }
            Assert(i == qi); Assert(j == qj); if (fPtr) Assert((*fPtr)[i * pd + j] == fij);
            solutions[k] += fij;
        }
        if (timing) sol.tmSubQuadPart = tm.Sec();
        sol.Init(solutions);
        if (pd >= 100) fprintf(stderr, "pd = %d, sd = %d, simple path solution: %lld + %lld = %lld ops\n", pd, sd, nOpsI, nOpsJ, nOpsI + nOpsJ);
        return sol;
    }

    if constexpr (true) // O((p + s) sqrt p) time
    {
        if (timing) tm.Reset();
        int sqrtP = 1; while (sqrtP * sqrtP < pd) ++sqrtP;
        vector<Query> queries(sd - 1);
        for (int k = 1; k < sd; ++k) { auto &Q = queries[k - 1]; Q.i = spST[k]; Q.j = pd - spPT[k]; Q.k = k; }
        vector<pair<int, int>> closestMarkedAncestor; 
        vector<int> firstQuery(pd); // for each marked u, we have a list of queries (i, j) for which u is the closest marked ancestor of i
        // Mark some nodes so that no node is more than sqrt(p) steps far from the closest marked ancestor.
        int maxSubtreeSize = sqrtP;
        int nMarked = iTree.MarkNodes(maxSubtreeSize, closestMarkedAncestor);
        for (auto &x : firstQuery) x = -1;
        long long costEstimate1 = nMarked * queries.size();
        long long costEstimate2 = 0;
        for (int qi = 0; qi < queries.size(); ++qi) { 
            auto &Q = queries[qi]; auto [anc, dist] = closestMarkedAncestor[Q.i];
            costEstimate2 += dist; Q.temp = firstQuery[anc]; firstQuery[anc] = qi; }
        if (pd > 100) fprintf(stderr, "pd = %d, sd = %d; maxSubtreeSize = %d -> %d marked nodes; cost estimate: %lld + %lld = %lld\n", pd, sd, maxSubtreeSize, nMarked, costEstimate1, costEstimate2, costEstimate1 + costEstimate2);
        // Calculate f(i, 0) for all i.
        vector<int> fi0(pd, 0);
        for (int i = 1, j = 0; i < pd; ++i) if (i + j + td >= pd)
        {
            int F = 0; if (int ii = iNext[i]; ii > 0) F += fi0[ii];
            if (int right = pd - i - td; right > 0 && right <= j && tOccInP[i])
                if (jTree.IsAncestorOf(right, j)) ++F;
            fi0[i] = F;
            if (fPtr) Assert((*fPtr)[i * pd + j] == F);
        }
        // Go through all the marked nodes.
        vector<int> fij(pd), ancPath(pd);
        for (int i = 0; i < pd; ++i) if (closestMarkedAncestor[i].first == i)
        {
            // Calculate f(i, j) for all j.
            fij[0] = fi0[i];
            for (int j = 1; j < pd; ++j)
            {
                int F = 0;
                if (i + j + td >= pd)
                {
                    if (int jj = jNext[j]; jj > 0) F += fij[jj];
                    if (int left = pd - j - td; left > 0 && left <= i && tOccInP[left])
                        if (iTree.IsAncestorOf(left, i)) ++F;
                }
                fij[j] = F;
                if (fPtr) Assert((*fPtr)[i * pd + j] == F);
            }
            // Go through all the queries (qi, qj) for which i is the closest marked ancestor of qi.
            for (int q = firstQuery[i]; q >= 0; ) {
                auto &Q = queries[q]; const int qi = Q.i, qj = Q.j;
                Assert(i == closestMarkedAncestor[qi].first);
                int nSteps = 0; for (int anc = qi; anc != i; anc = iNext[anc]) ancPath[nSteps++] = anc;
                Assert(nSteps == closestMarkedAncestor[qi].second);
                int F = fij[qj]; // Now F = f(i, qj).
                for (int step = 0; step < nSteps; ++step)
                {
                    int u = ancPath[nSteps - 1 - step];
                    int uu = iNext[u]; if (step == 0) Assert(uu == i); else Assert(uu == ancPath[nSteps - step]);
                    // Now F = f(uu, qj).
                    if (u + qj + td >= pd) if (int right = pd - u - td; right > 0 && right <= qj && tOccInP[u])
                        if (jTree.IsAncestorOf(right, qj)) ++F;
                    if (fPtr) Assert((*fPtr)[u * pd + qj] == F);
                    // Now F = f(u, qj).
                }
                // Now F = f(qi, qj).
                Q.result = F;
                q = Q.temp; // next query for the current marked ancestor
            }
        }
        if (timing) sol.tmSubQuadPart = tm.Sec();
        // Store the results.
        for (auto &Q : queries) solutions[Q.k] += Q.result;
        sol.Init(solutions);
        return sol;
    }

    if constexpr (false) // use solutions calculated by the quadratic method
        for (int k = 1; k < sd; ++k)
        {
            int i = spST[k], j = pd - spPT[k];
            Assert(0 <= i); Assert(i < pd); Assert(0 <= j); Assert(j < pd);
            if (i > 0 && j > 0) solutions[k] += f[i * pd + j];
        }

    if (timing) tm.Reset();
    iTree.PartitionTree(); jTree.PartitionTree();
    sol.qiMax = 0; sol.qjMax = 0;
    QueryProcessor QP{iTree, jTree, tOccInP, td, pd, fPtr}; QP.queries.resize(sd - 1);
    for (int k = 1; k < sd; ++k) {
        auto &Q = QP.queries[k - 1];
        Q.i = spST[k]; Q.j = pd - spPT[k]; Q.k = k; 
        sol.qiMax = max(sol.qiMax, Q.i); sol.qjMax = max(sol.qjMax, Q.j); }
    QP.Process();
    for (auto &Q : QP.queries) solutions[Q.k] += Q.result;
    if (timing) sol.tmSubQuadPart = tm.Sec();
    sort(QP.queries.begin(), QP.queries.end(), [] (const auto &x, const auto &y) { return x.i < y.i || (x.i == y.i && x.j < y.j); });
    sol.nDifQueries = 0; for (int i = 1; i < QP.queries.size(); ++i) if (QP.queries[i].i != QP.queries[i - 1].i || QP.queries[i].j != QP.queries[i -1].j) ++sol.nDifQueries;
    sol.opCountRec = QP.opCountRec; sol.opCountS = QP.opCountS;
    sol.Init(solutions);
    if constexpr (DEBUG) if (sd >= 30 || td >= 30 || pd >= 30) fprintf(stderr, "sd = %d, td = %d, pd = %d; opCount: %d + %d = %d (quadratic: %lld) [ms times: lin %.2f, quad %.2f, subQuad %.2f]; %d dif queries, max qi %d qj %d -> %lld; best sol %d at %d k's (min %d, max %d); interest %.3f\n",
        sd, td, pd, QP.opCountRec, QP.opCountS, QP.opCountRec + QP.opCountS, ((long long) pd) * pd, sol.tmLinPart * 1000.0, sol.tmQuadPart * 1000.0, sol.tmSubQuadPart * 1000.0, sol.nDifQueries, sol.qiMax, sol.qjMax, ((long long) sol.qiMax) * sol.qjMax, sol.bestSol, sol.bestCount, sol.bestMin, sol.bestMax, sol.Interestingness());
    return sol;
}

void SolveNaive(const string &s, const string &t, const string &p, vector<int> &solutions)
{
    const int sd = s.length(), td = t.length(), pd = p.length();
    const int wd = sd + td; string w; w.resize(wd); 
    solutions.resize(sd + 1);
    for (int k = 0; k <= sd; ++k)
    {
        for (int i = 0; i < k; ++i) w[i] = s[i];
        for (int i = 0; i < td; ++i) w[k + i] = t[i];
        for (int i = k; i < sd; ++i) w[i + td] = s[i];
        int count = 0;
        for (int i = 0; i + pd <= wd; ++i)
        {
            int j = 0; while (j < pd && w[i + j] == p[j]) ++j;
            if (j >= pd) ++count;
        }
        solutions[k] = count;
    }
}

Solution TestSolve(const string &s, const string &t, const string &p)
{
    vector<int> sol1, sol2;
    Solution solution = Solve(s, t, p, sol1);
    bool doNaive = s.size() <= 500 && p.size() <= 500 && t.size() <= 500;
    if (doNaive) SolveNaive(s, t, p, sol2);
    int bestSol = -1, bestCount = 0, bestMin = -1, bestMax = -1;
    for (int k = 0, sd = s.size(); k <= sd; ++k) {
        if (sol1[k] > bestSol) bestSol = sol1[k], bestCount = 0, bestMin = k;
        if (sol1[k] == bestSol) ++bestCount, bestMax = k; }
    auto Trivial = [] (const string &x) { for (int i = 1; i < x.size(); ++i) if (x[0] != x[i]) return false; return true; };
    //if (false) if (p.size() > 2 && bestCount > 2 && bestSol >= 2 && p != s && p != t && ! Trivial(s) && ! Trivial(p) && ! Trivial(t) && bestMax < s.size() && bestMin > 0) 
    //if (Trivial(s + p + t)) 
    // printf("s = \"%s\", t = \"%s\", p = \"%s\", best sol = %d at %d k's (min %d, max %d)\n", s.c_str(), t.c_str(), p.c_str(), bestSol, bestCount, bestMin, bestMax);
    if (false)
        printf("sd = %d, td = %d, pd = %d; best sol = %d at %d k's (min %d, max %d)\n", (int) s.size(), (int) t.size(), (int) p.size(), bestSol, bestCount, bestMin, bestMax);
    if (doNaive) for (int k = 0, sd = s.size(); k <= sd; ++k) 
    {
        if (sol1[k] != sol2[k]) { fprintf(stderr, "s = \"%s\", t = \"%s\", p = \"%s\", sol[%d] = %d or %d\n", 
            s.c_str(), t.c_str(), p.c_str(), k, sol1[k], sol2[k]); fflush(stderr); }
        Assert(sol1[k] == sol2[k]);
    }
    return solution;
}

void TestSolve()
{
    constexpr int M = 7;
    for (int sd = 1; sd <= M; ++sd) for (int td = 1; td <= M; ++td) for (int pd = td; pd <= M; ++pd)
    {
        //if (! (sd == 3 && td == 1 && pd == 3)) continue;
        //if (sd < 4 || td < 4 || pd < 8) continue;
        string s, t, p; s.resize(sd); t.resize(td); p.resize(pd);
        printf("%d %d %d      \n", sd, td, pd); fflush(stdout);
        for (int sb = 0; sb < (1 << sd); ++sb)
        {
            for (int i = 0; i < sd; ++i) s[i] = 'a' + ((sb >> i) & 1);
            for (int tb = 0; tb < (1 << td); ++tb)
            {
                for (int i = 0; i < td; ++i) t[i] = 'a' + ((tb >> i) & 1);
                for (int pb = 0; pb < (1 << pd); ++pb)
                {
                    for (int i = 0; i < pd; ++i) p[i] = 'a' + ((pb >> i) & 1);
                    TestSolve(s, t, p);
                }
            }
        }
    }
}

void CycFill(string &s, int i) { for (auto &c : s) { c = 'a' + i; i = (i + 1) % 2; } }
void ChFill(string &s, char ch = 'a') { for (auto &c : s) c = ch; }

struct TestCase { 
    string s, t, p; 
    TestCase() = default;
    TestCase(string S, string T, string P) : s(move(S)), t(move(T)), p(move(P)) { }
};

template<typename R> int Rnd(R &&r, int minVal, int maxVal) { return uniform_int_distribution<int>(minVal, maxVal)(r); }

void TestSolve2()
{
    for (int nIter = 0; ; ++nIter)
    {
        mt19937_64 r(123 + nIter);
        //auto d = uniform_int_distribution<int>(10, 50);
        auto d = uniform_int_distribution<int>(100, 500);
        //auto d = uniform_int_distribution<int>(300, 1500);
        //auto d = uniform_int_distribution<int>(1000, 5000);
        //auto d = uniform_int_distribution<int>(10000, 50000);
        //auto d = uniform_int_distribution<int>(90000, 100000);
        int sd = d(r), td = d(r), pd = d(r); if (pd < td) continue;
        string s, t, p; s.resize(sd); t.resize(td); p.resize(pd);
        if (false)
        {
            for (auto &c : s) c = 'a';  for (auto &c : p) c = 'a';  for (auto &c : t) c = 'a'; 
            TestSolve(s, t, p);
            for (int i = 0; i < sd / 10; ++i) s[uniform_int_distribution<int>(0, sd - 1)(r)] = 'b';
            TestSolve(s, t, p);
            auto d2 = uniform_int_distribution<int>(0, 1);
            for (auto &c : s) c = 'a' + d2(r);  for (auto &c : t) c = 'a' + d2(r);  for (auto &c : p) c = 'a' + d2(r); 
            TestSolve(s, t, p);
            for (auto &c : s) c = 'a';  for (auto &c : p) c = 'a';  for (auto &c : t) c = 'a'; 
            s[uniform_int_distribution<int>(0, s.size() - 1)(r)] = 'b';
            p[uniform_int_distribution<int>(0, p.size() - 1)(r)] = 'b';
            TestSolve(s, t, p);
        }
        if (false)
        {
            CycFill(s, 0); CycFill(t, 0); CycFill(p, 0);
            TestSolve(s, t, p);
            CycFill(s, 0); CycFill(t, 0); CycFill(p, 1);
            TestSolve(s, t, p);
            CycFill(s, 0); CycFill(t, 1); CycFill(p, 0);
            TestSolve(s, t, p);
            CycFill(s, 0); CycFill(t, 1); CycFill(p, 1);
            TestSolve(s, t, p);
        }
        if (true)
        {
            CycFill(s, 0); CycFill(t, 0); CycFill(p, 0);
            for (int i = 0; i < sd / 10; ++i) s[uniform_int_distribution<int>(0, sd - 1)(r)] = 'b';
            TestSolve(s, t, p);
            CycFill(s, 0); CycFill(t, 0); CycFill(p, 0);
            int i = uniform_int_distribution<int>(1, s.size() - 1)(r); swap(s[i - 1], s[i]);
            i = uniform_int_distribution<int>(1, p.size() - 1)(r); swap(p[i - 1], p[i]);
            TestSolve(s, t, p);
        }
    }
}

void SaveBatch(mt19937_64 &r, int batchNo, const vector<TestCase> &cases)
{
    for (int i = 0; i < cases.size(); ++i)
    {
        char buf[50]; sprintf(buf, "insertion-tests\\insertion.%02d.%02d.in", batchNo, i + 1);
        auto C = cases[i];
        char xlat[26]; for (int i = 0; i < 26; ++i) { int j = Rnd(r, 0, i); xlat[i] = xlat[j]; xlat[j] = 'a' + i; }
        for (auto &c : C.s) c = xlat[c - 'a'];
        for (auto &c : C.t) c = xlat[c - 'a'];
        for (auto &c : C.p) c = xlat[c - 'a'];
        FILE *f = fopen(buf, "wt");
        fprintf(f, "%s\n%s\n%s\n", C.s.c_str(), C.t.c_str(), C.p.c_str());
        fclose(f);
    }
}

vector<TestCase> GenSmall(int MaxLen, int nCases)
{
    vector<TestCase> cases;
    mt19937_64 r(567);
    for (int i = 0; i < nCases; ++i)
    {
        TestCase bestCand; Solution bestSol;
        // The last half of the cases are purely random, to avoid interestingness bias.
        for (int j = 0; j < (i > nCases / 2 ? 1 : 1000); ++j)
        {
            int sd = Rnd(r, 1, MaxLen), td = Rnd(r, 1, MaxLen), pd = Rnd(r, 1, MaxLen);
            if ((i % 2) ? (td < pd) : (td > pd)) swap(td, pd); // half the cases have |t| < |p|, half |t| > |p|
            string s(sd, 'a'), t(td, 'a'), p(pd, 'a');
            auto d2 = uniform_int_distribution<int>(0, 1);
            for (auto &c : s) c = 'a' + d2(r);  for (auto &c : t) c = 'a' + d2(r);  for (auto &c : p) c = 'a' + d2(r); 
            vector<int> dummy;
            Solution sol = Solve(s, t, p, dummy);
            if (j == 0 || sol.Interestingness() > bestSol.Interestingness())
                bestCand = TestCase{s, t, p}, bestSol = sol;
        }
        printf("[i = %d] sd = %d, td = %d, pd = %d; sol = %d, num %d, min %d, max %d; interest %.3f\n", i,
            int(bestCand.s.length()), int(bestCand.t.length()), int(bestCand.p.length()),
            bestSol.bestSol, bestSol.bestCount, bestSol.bestMin, bestSol.bestMax, bestSol.Interestingness());
        cases.push_back(move(bestCand));
    }
    return cases;
}

vector<vector<TestCase>> Gen(int MaxLen)
{
    vector<vector<TestCase>> batches;
    mt19937_64 r(123);
    for (int lenCombo = 0; lenCombo < 3; ++lenCombo)
    {
        int sd, td, pd;
        // long s, medium t, long p
        if (lenCombo == 0) sd = Rnd(r, (MaxLen / 10) * 9, MaxLen), td = Rnd(r, (MaxLen / 10) * 4, (MaxLen / 10) * 6), pd = Rnd(r, (MaxLen / 10) * 9, MaxLen);
        // long s, long t, long p (longer than t)
        else if (lenCombo == 1) sd = Rnd(r, (MaxLen / 10) * 9, MaxLen), td = Rnd(r, (MaxLen / 10) * 8, (MaxLen / 10) * 9), pd = Rnd(r, (MaxLen / 10) * 9, MaxLen);
        // long s, long t, long p (shorter than t)
        else sd = Rnd(r, (MaxLen / 10) * 9, MaxLen), pd = Rnd(r, (MaxLen / 10) * 8, (MaxLen / 10) * 9), sd = Rnd(r, (MaxLen / 10) * 9, MaxLen);
        string s(sd, 'a'), t(td, 'a'), p(pd, 'a');
        vector<TestCase> tests;
        tests.emplace_back(s, t, p);
        //for (int i = 0; i < sd / 10; ++i) s[Rnd(mt19937_64(123 + iter), 0, sd - 1)] = 'b';
        //tests.emplace_back(s, t, p);
        vector<pair<TestCase, Solution>> bestTests;
        for (int nIter = 0; nIter < 10; ++nIter)
        {
            vector<TestCase> testCands;
            if (true)
            {
                // s = mostly a's, 10% b's; t and p = all a's.  Not very intersting.
                ChFill(s); ChFill(t); ChFill(p);
                for (int i = 0; i < sd / 10; ++i) s[Rnd(r, 0, sd - 1)] = 'b';
                testCands.emplace_back(s, t, p);
                // Purely random strings of 50% a's, 50% b's.  Quite uninteresting since p never occurs.
                auto d2 = uniform_int_distribution<int>(0, 1);
                for (auto &c : s) c = 'a' + d2(r);  for (auto &c : t) c = 'a' + d2(r);  for (auto &c : p) c = 'a' + d2(r); 
                testCands.emplace_back(s, t, p);
                // s = all a's, except for one b; p = likewise; t = all a's.  This is quite interesting.
                ChFill(s); ChFill(t); ChFill(p);
                s[Rnd(r, 0, sd - 1)] = 'b'; p[Rnd(r, 0, pd - 1)] = 'b';
                testCands.emplace_back(s, t, p);
                // s = (ab)*, t likewise, p = (ba)*.  Not very interesting.
                CycFill(s, 0); CycFill(t, 0); CycFill(p, 1);
                testCands.emplace_back(s, t, p);
                // s = (ab)* but with one extra b; t = pure (ab)*, p = (ba)*.  Somewhat interesting.
                CycFill(s, 0); CycFill(t, 0); CycFill(p, 1);
                for (int i = 0; i < 3; ++i) s[Rnd(r, 0, sd - 1)] = 'b';
                testCands.emplace_back(s, t, p);
            }
            //
            for (int i = 0; i < testCands.size(); ++i)
            {
                const auto &testCase = testCands[i]; vector<int> dummy;
                Solution sol = Solve(testCase.s, testCase.t, testCase.p, dummy);
                if (i >= bestTests.size()) bestTests.emplace_back(testCase, sol);
                else if (sol.Interestingness() > bestTests[i].second.Interestingness()) { bestTests[i].first = testCase; bestTests[i].second = sol; }
            }
        }
        for (auto [testCase, sol] : bestTests) 
        {
            tests.emplace_back(testCase);
            printf("Sol: best %d, num %d, min %d, max %d\n", sol.bestSol, sol.bestCount, sol.bestMin, sol.bestMax);
        }
        batches.push_back(move(tests));
    }
    return batches;
}

void GenAll()
{
    vector<vector<TestCase>> batches;
    batches.push_back(GenSmall(10, 20));
    for (auto batch : Gen(1000)) batches.push_back(move(batch));
    for (auto batch : Gen(100000)) batches.push_back(move(batch));
    for (int batchNo = 0; batchNo < batches.size(); ++batchNo) 
    {
        mt19937_64 r(678 + batchNo);
        SaveBatch(r, batchNo + 1, batches[batchNo]);
    } 
}

int cercMain()
{
    constexpr int MaxS = 100'000, MaxT = 100'000, MaxP = 100'000;
    string s, t, p; 
    getline(cin, s); getline(cin, t); getline(cin, p);
    Assert(1 <= s.length()); Assert(s.length() <= MaxS);
    for (char c : s) { Assert('a' <= c); Assert(c <= 'z'); }
    Assert(1 <= t.length()); Assert(t.length() <= MaxT);
    for (char c : t) { Assert('a' <= c); Assert(c <= 'z'); }
    Assert(1 <= p.length()); Assert(p.length() <= MaxP);
    for (char c : p) { Assert('a' <= c); Assert(c <= 'z'); }
    vector<int> v; Solve(s, t, p, v);
    Solution sol{v};
    cout << sol.bestSol << ' ' << sol.bestCount << ' ' << sol.bestMin << ' ' << sol.bestMax << endl;
    return 0;
}

int main()
{
    //freopen("insertion-tests\\insertion.06.05.in", "rt", stdin);
    //TestBuildSuffixTable(); return 0;
    //TestBuildSuffixTable2(); return 0;
    //TestBuildOccCounts(); return 0;
    //TestBuildPrefixTable(); return 0;
    //TestBuildPrefixTable2(); return 0;
    //TestBuildPrefixTable2B(); return 0;
    //TestBuildSuffixTable2B(); return 0;
    //TestPartitionTree(); return 0;
    //TestSolve(); return 0;
    //Timer tm(true); for (int i = 0; ; ++i) { while (tm.Sec() < i) ; printf("%.2f  \r", tm.Sec()); }
    //TestSolve2(); return 0;
    //Gen(100000); return 0;
    //GenAll(); return 0;
    return cercMain();
}