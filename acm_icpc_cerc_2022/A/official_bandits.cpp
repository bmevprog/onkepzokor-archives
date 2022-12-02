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

typedef long long int llint;

mt19937_64 treapRnd(123);
int GetRndPriority() { return uniform_int_distribution<int>()(treapRnd); }

struct TreapNode
{
    llint key;
    int priority, left = -1, right = -1, size = 1; 
    TreapNode() = default;
    explicit TreapNode(llint key_, int priority_) : key(key_), priority(priority_) { }
    void RecalcSize();
};

vector<TreapNode> treapNodes;

void TreapNode::RecalcSize() { size = 1 + (left < 0 ? 0 : treapNodes[left].size) + (right < 0 ? 0 : treapNodes[right].size); }

constexpr bool DEBUG = false;
constexpr int MaxN = 100'000, MaxQ = 100'000, MaxR = 1'000'000'000, MaxC = 1'000'000'000;

struct Treap
{
    int root = -1;
    vector<llint> keys;
protected:
    int Add(llint key, int parent)
    {
        if (parent < 0) {
            int newNode = treapNodes.size(); treapNodes.emplace_back(key, GetRndPriority()); return newNode; }
        if (key >= treapNodes[parent].key) 
        {
            int newRight = Add(key, treapNodes[parent].right);
            auto &P = treapNodes[parent], &NR = treapNodes[newRight];
            if (NR.priority > P.priority) { P.right = NR.left; NR.left = parent;
                P.RecalcSize(); NR.RecalcSize(); return newRight; }
            else { P.right = newRight; P.RecalcSize(); return parent; }
        }
        else
        {
            int newLeft = Add(key, treapNodes[parent].left);
            auto &P = treapNodes[parent], &NL = treapNodes[newLeft];
            if (NL.priority > P.priority) { P.left = NL.right; NL.right = parent;
                P.RecalcSize(); NL.RecalcSize(); return newLeft; }
            else { P.left = newLeft; P.RecalcSize(); return parent; }
        }
    }
public:
    void Add(llint key) { 
        Assert(key >= 0); Assert(key <= MaxR);
        // Note: key = 0 also has to be added since we can have legitimate 0-length edges.
        if constexpr (DEBUG) keys.emplace_back(key);
        root = Add(key, root); 
    }
    int CountGe(llint key) // count the nodes whose key is >= 'key'
    {
        if (key <= 0) return (root < 0) ? 0 : treapNodes[root].size;
        if (key > MaxR) return 0;
        int node = root, result = 0;
        while (node >= 0)
        {
            auto &N = treapNodes[node];
            if (key > N.key) { node = N.right; continue; }
            if (N.right >= 0) result += treapNodes[N.right].size; // all those are > key
            ++result; // because key <= N.key
            node = N.left; 
        }
        if constexpr (DEBUG)
        {
            int result2 = 0; for (int x : keys) if (x >= key) ++result2;
            Assert(result2 == result);
        }
        return result;
    }
};

struct Vertex
{
    vector<pair<int, int>> neigh; // (neighbour, distance to neighbour) pairs
    int subtreeSize; // Used temporarily during centroid decomposition.
    int level = -1; // the level of centroid decomposition at which this node is a centroid
    int parentCentroid = -1;
    int parent;  // used temporarily during the centroid decomposition
    Treap treap, contribToParent;
    vector<int> neighEdges_;
};
int n, root;
vector<Vertex> verts;
vector<tuple<int, int, int>> roads; // u, v, d_uv
vector<vector<llint>> centDist; // centDist[level][u] = the distance from 'u' to its centroid at that level; valid for 'level <= verts[u].level'.
vector<vector<int>> centTbl_; // centTbl_[level][u] = the centroid at level 'level' whose tree includes 'u'; only for debugging purposes

// 'parentCentroid' is a neighbour of 'startNode'; we're exploring 'startNode' and all nodes
// that can be reached from it without going through 'parentCentroid'.
int CentroidDecomposition(int startNode, int parentCentroid)
{
    stack<pair<int, bool>> toDo;
    vector<int> postOrderTraversal;
    // Explore the tree from 'startNode' and compute the size of every subtree.
    // Temporarily, each node 'parent' will be its predecessor on the path from 'startNode' to that node.
    int level = (parentCentroid < 0) ? 0 : verts[parentCentroid].level + 1;
    toDo.emplace(startNode, true); verts[startNode].parent = -1;
    while (! toDo.empty())
    {
        auto [u, enter] = toDo.top(); auto &U = verts[u]; 
        if (! enter) { 
            if (U.parent >= 0) verts[U.parent].subtreeSize += U.subtreeSize;
            postOrderTraversal.emplace_back(u);
            toDo.pop(); continue; }
        toDo.top().second = false;
        U.subtreeSize = 1; 
        for (auto [v, d] : U.neigh) {
            if (v == U.parent) continue;
            auto &V = verts[v]; if (V.level >= 0) { Assert(V.level < level); continue; }
            V.parent = u; toDo.emplace(v, true); }
    }
    // Determine which node is the centroid.
    int subtreeSize = postOrderTraversal.size();
    int centroid = -1;
    for (int u : postOrderTraversal) if (2 * verts[u].subtreeSize >= subtreeSize) { centroid = u; break; }
    Assert(centroid >= 0);
    auto &C = verts[centroid]; C.parentCentroid = parentCentroid; C.level = level;
    // Calculate the distances from the nodes to this centroid.
    while (centDist.size() <= level) centDist.emplace_back(n, -1);
    while (centTbl_.size() <= level) centTbl_.emplace_back(n, -1);
    auto &D = centDist[level]; D[centroid] = 0;
    Assert(toDo.empty()); toDo.emplace(centroid, true); C.parent = -1;
    while (! toDo.empty())
    {
        auto [u, enter] = toDo.top(); auto &U = verts[u];
        if (! enter) { toDo.pop(); continue; }
        toDo.top().second = false; centTbl_[level][u] = centroid;
        for (auto [v, d] : U.neigh) {
            if (v == U.parent) continue;
            auto &V = verts[v]; if (V.level >= 0) { Assert(V.level < level); continue; }
            V.parent = u; D[v] = D[u] + d; toDo.emplace(v, true); }
    }
    // Proceed recursively in each part.
    for (auto [v, d] : C.neigh) if (verts[v].level < 0) CentroidDecomposition(v, centroid);
    return centroid;
}

int Query(int roadNo)
{
    auto [u, v, d_uv] = roads[roadNo];
    if (verts[u].level < verts[v].level) swap(u, v);
    auto &U = verts[u], &V = verts[v];
    Assert(centTbl_[V.level][u] == v);
    // First, all the contracts coming from u's tree at its own level that reach v (which is
    // at a higher level (i.e. numerically lower level) of decomposition).
    int result = U.treap.CountGe(d_uv);
    for (int w = u; ; )
    {
        auto &W = verts[w]; int p = W.parentCentroid; if (p < 0) { Assert(W.level == 0); break; }
        auto &P = verts[p]; Assert(P.level == W.level - 1); Assert(centTbl_[P.level][u] == p);
        llint d_wp = centDist[P.level][w];
        if (P.level > V.level)
        {
            Assert(centTbl_[V.level][p] == v);
            // p is at a lower (numerically higher) level of decomposition than v.
            // Hence the path from p to v must run though u (because u is a neighbour
            // of v, so if there were another way to reach v from p, there would be
            // a cycle in the graph).  So dist(p, u) < dist(p, v), which means that
            // in P, we have to query for contracts that reach v to make sure that the
            // whole road segment is covered.
            llint queryDist = centDist[P.level][u] + d_uv;
            result += P.treap.CountGe(queryDist);
            // However, some of these contracts in P came from W and were already counted earlier.
            result -= W.contribToParent.CountGe(queryDist);
        }
        else if (P.level == V.level)
        {
            Assert(p == v); 
            // Now we're in v; add all contracts that reach u.
            result += P.treap.CountGe(d_uv);
            // However, some of these contracts came from W and were already counted there.
            result -= W.contribToParent.CountGe(d_uv);
        }
        else
        {
            Assert(P.level < V.level);
            Assert(centTbl_[P.level][v] == p);
            // p is at a higher (numerically lower) level of decomposition than v.
            // Here it's impossible to say in general which of u and v is closer to p;
            // but since p is at a higher level than both, we have the distances to both
            // in centDist.
            Assert(abs(centDist[P.level][u] - centDist[P.level][v]) == d_uv);
            llint dpMax = max(centDist[P.level][u], centDist[P.level][v]);
            result += P.treap.CountGe(dpMax);
            // However, some of these contracts came from W and were already counted there.
            Assert(centTbl_[W.level][v] == w);
            Assert(centTbl_[W.level][u] == w);
            Assert(abs(centDist[W.level][u] - centDist[W.level][v]) == d_uv);
            llint dwMax = max(centDist[W.level][u], centDist[W.level][v]);
            result -= W.contribToParent.CountGe(dpMax); // (dwMax + 2 * d_wp);
        }
        w = p;
    }
    return result;
}

int main()
{
    //freopen("bandits99.in", "rt", stdin);
    //freopen("bandits-tests\\big03.in", "rt", stdin);
    //freopen("bandits-tests\\shallow02.in", "rt", stdin);
    //freopen("bandits-tests\\line02.in", "rt", stdin);
    // Read the tree.
    int ok_ = scanf("%d", &n); Assert(ok_ == 1);
    Assert(1 <= n); Assert(n <= MaxN);
    verts.clear(); verts.resize(n);
    vector<int> edgeCoverage_; if constexpr (DEBUG) edgeCoverage_.resize(n - 1, 0);
    for (int i = 0; i < n - 1; ++i) 
    {
        int a, b, c; ok_ = scanf("%d %d %d", &a, &b, &c);
        Assert(1 <= a); Assert(a <= n); Assert(a != b);
        Assert(1 <= b); Assert(b <= n);
        Assert(0 <= c); Assert(c <= MaxC);
        --a; --b; roads.emplace_back(a, b, c);
        verts[a].neigh.emplace_back(b, c); verts[b].neigh.emplace_back(a, c);
        if constexpr (DEBUG) { verts[a].neighEdges_.emplace_back(i); verts[b].neighEdges_.emplace_back(i); }
    }
    // Make sure that all the edges were different.
    if constexpr (DEBUG) for (auto &V : verts) {
        sort(V.neigh.begin(), V.neigh.end());
        for (int i = 1; i < V.neigh.size(); ++i) Assert(V.neigh[i - 1].first != V.neigh[i].first); }
    // Perform centroid decomposition.
    root = 0; int centroid = CentroidDecomposition(root, -1);
    for (auto [u,v, d] : roads) Assert(verts[u].level != verts[v].level);
    // Process the queries.
    int nQueries; ok_ = scanf("%d", &nQueries); Assert(ok_ == 1);
    Assert(1 <= nQueries); Assert(nQueries <= MaxQ);
    while (nQueries-- > 0)
    {
        char s[2]; ok_ = scanf("%1s", &s[0]); Assert(ok_ == 1); 
        if (s[0] == '+') 
        { 
            // A new contract is being added.
            int u, r; ok_ = scanf("%d %d", &u, &r); Assert(ok_ == 2);
            Assert(1 <= u); Assert(u <= n); Assert(0 <= r); Assert(r <= MaxR);
            --u;
            // Update the centroids from u upwards.
            for (int v = u, prev = -1; v >= 0; )
            {
                auto &V = verts[v]; 
                Assert(v == centTbl_[V.level][u]);
                if (llint dist = r - centDist[V.level][u]; dist >= 0) {
                    V.treap.Add(dist);
                    if (prev >= 0) verts[prev].contribToParent.Add(dist); }
                prev = v; v = V.parentCentroid;
            }
            if constexpr (DEBUG)
            {
                // Perform a breadth-first search from 'u' to mark all edges affected by this contract.
                struct Entry { int v, pred, r; };
                stack<Entry> toDo; toDo.push({u, -1, r});
                while (! toDo.empty())
                {
                    auto ve = toDo.top(); toDo.pop(); int v = ve.v;
                    for (int edgeNo : verts[v].neighEdges_)
                    {
                        auto [e1, e2, eD] = roads[edgeNo]; Assert(e1 == v || e2 == v);
                        if (eD > ve.r) continue;
                        int w = (e1 == v) ? e2 : e1; if (w == ve.pred) continue;
                        edgeCoverage_[edgeNo] += 1;
                        toDo.push({w, v, ve.r - eD}); // even if ve.r == eD, we have to continue because it's possible to have 0-length edges
                    }
                }
                // Make sure that all queries would still return the correct result at this point.
                if (false) for (int roadNo = 0; roadNo < n - 1; ++roadNo)
                {
                    int result = Query(roadNo);
                    if (result != edgeCoverage_[roadNo])
                    {
                        printf("%d!", Query(roadNo));
                    }
                    Assert(result == edgeCoverage_[roadNo]);
                }
            }
        }
        else 
        {
            Assert(s[0] == '?');
            // We're being queried about a road.
            int j; ok_ = scanf("%d", &j); Assert(1 <= j); Assert(j <= n - 1);
            int result = Query(j - 1);
            if constexpr (DEBUG) {
                if (result != edgeCoverage_[j - 1]) 
                    printf("%d!", Query(j - 1));
                Assert(result == edgeCoverage_[j - 1]);
            }
            printf("%d\n", result);
        }
    }
}