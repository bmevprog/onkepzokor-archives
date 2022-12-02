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

typedef long long int llint;
constexpr llint MaxN = 200'000, MaxM = 200'000, MinA = -1'000'000'000, MaxA = 1'000'000'000;

struct Point
{
    llint x, y;
    Point() = default;
    Point(llint X, llint Y) : x(X), y(Y) { }
};

Point operator - (const Point &a, const Point &b) { return Point(a.x - b.x, a.y - b.y); }

llint Ccw(const Point &a, const Point &b)
{
    //  QW QW QW QW  This could lead to overflows.  If we have 2 * 10^5 days with 10^9 income each day,
    // the cumulative sums will reach 2 * 10^14.  The sort of delta-x that will be involved in a CCW call
    // can reach 10^6 (max. number of days).  Thus the products in the line below will be up to 2 * 10^20,
    // which is too big for 64 bits.
    auto d = a.x * b.y - b.x * a.y;
    if constexpr (true)
    {
        int result1 = d > 0 ? 1 : d < 0 ? -1 : 0;
        double z = b.y / double(b.x) - a.y / double(a.x); int result3 = (z > 0) ? 1 : (z < 0) ? -1 : 0;
        //typedef _BitInt(128) Int128;
        typedef __int128 Int128;
        Int128 d128 = a.x * Int128(b.y) - Int128(a.y) * b.x;
        int result4 = (d128 > 0) ? 1 : (d128 < 0) ? -1 : 0;
        long long byLo = b.y & 0xffffffffLL, byHi = b.y >> 32;
        long long ayLo = a.y & 0xffffffffLL, ayHi = a.y >> 32;
        long long leftLo = byLo * a.x, rightLo = b.x * ayLo;
        long long leftHi = byHi * a.x + (leftLo >> 32), rightHi = b.x * ayHi + (rightLo >> 32);
        leftLo &= 0xffffffffLL; rightLo &= 0xffffffffLL;
        int result2 = leftHi > rightHi ? 1 : leftHi < rightHi ? -1 : leftLo > rightLo ? 1 : leftLo < rightLo ? -1 : 0;
        //Assert(result3 == result2); // do floating-point imprecisions lead to wrong results? hoho, sometimes they do!
        Assert(result4 == result2); // 128-bit ints should work too
        //Assert(result1 == result2); // it turns out that overflows do indeed occur
        return result2; // correct result (int96 or int128)
        //return result1; // int64 result
        //return result3; // double result
    }
    return d > 0 ? 1 : d < 0 ? -1 : 0;
}

int Ccw(const Point &r, const Point &a, const Point &b) { return Ccw(a - r, b - r); }

struct Node
{
    // If this is node 'tree[L][x], it represents the sequence of points pts[0][x << L..(x + 1) << L - 1].
    // Their convex hull is stored in pts[L][x << L..x << L + d-1].
    int d;
};

class Solver
{
    int n;
    // pts[0][x] = {x, y} where y is the cumulative income in the first x months.
    // pts[L] for L > 0 stores convex hulls of groups of 2^L points.
    vector<vector<Point>> pts;
    vector<vector<Node>> tree;

    template<typename SrcIt, typename DestIt>
    int Merge(DestIt dest, SrcIt src1, int d1, SrcIt src2, int d2)
    {
        int d = d1; for (int i = 0; i < d1; ++i) dest[i] = src1[i];
        for (int i = 0; i < d2; ++i)
        {
            while (d > 1 && Ccw(dest[d - 2], dest[d - 1], src2[i]) < 0) --d;
            dest[d++] = src2[i];
        }
        return d;
    }

    int MaxLoan(int level, int nodeIdx, const Point &T)
    {
        const auto &node = tree[level][nodeIdx];
        const int d = node.d; const auto p = pts[level].begin() + (nodeIdx << level);
        int L = 0, R = d;
        while (L < R)
        {
            // Points 0..L-1 lie above the line from T to L.
            // Points R+1..d-1 lie above the line from T to R.
            int M = (L + R) / 2;
            if (M == d - 1 || Ccw(T, p[M], p[M + 1]) > 0) 
                // M+1 is above the line from T to M.  Hence the points M+1..R are above that line too.
                R = M;
            else 
                // M is above the line from T to M+1.  Hence the points L..M-1 are above that line too.
                L = M + 1;
        }
        if constexpr (false)
        {
            for (int i = 0; i < (1 << level); ++i)
            {
                int x = (nodeIdx << level) + i;
                const auto &P = pts[0][x];
                Assert(Ccw(T, p[L], P) >= 0);
            }
        }
        Point dir = p[L] - T;
        // We want to return the largest integer loan, i.e. floor(dir.y / dir.x).
        // Since the daily incomes can be negative, dir.y could be negative and 
        // integer division would round upwards, which we presumably don't want.  QW QW QW clarify the problem statement
        Assert(dir.x > 0);
        if (dir.y < 0) return -((-dir.y + dir.x - 1) / dir.x); // floor(z) = -ceil(-z)
        else return dir.y / dir.x; // largest integer loan
    }

public:
    explicit Solver(const vector<int> &monthlyIncome)
    {
        n = monthlyIncome.size();
        // Prepare the lowest level of the tree.
        pts.emplace_back(n + 1); tree.emplace_back(n + 1);
        pts[0][0] = {0, 0};
        for (int x = 1; x <= n; ++x) {
            auto &T = pts[0][x]; T.x = x; T.y = pts[0][x - 1].y + monthlyIncome[x - 1];
            auto &N = tree[0][x]; N.d = 1; }
        // Build higher levels of the tree.
        for (int level = 0; tree[level].size() > 1; ++level)
        {
            int nPrev = tree[level].size();
            int nCur = nPrev / 2; pts.emplace_back(n + 1); tree.emplace_back(nCur);
            auto &prevT = tree[level], &curT = tree[level + 1];
            auto &prevP = pts[level], &curP = pts[level + 1];  
            for (int i = 0; i < nCur; ++i) {
                auto &dest = curT[i]; auto &src1 = prevT[2 * i], &src2 = prevT[2 * i + 1];
                // Merge the hulls 'src1' and 'src2' into 'dest'.
                dest.d = Merge(curP.begin() + ((2 * i) << level), prevP.begin() + ((2* i) << level), src1.d, prevP.begin() + ((2 * i + 1) << level), src2.d); }
        }
    }
    int MaxLoan(const int s, const int k)
    {
        int xL = s, xR = xL + k;
        Point pt0 = pts[0][xL - 1];
        int maxLoan = MaxA + 1;
        for (int level = 0; xL < xR; ++level)
        {
            if ((xL & 1) == 1) maxLoan = min(maxLoan, MaxLoan(level, xL++, pt0)); 
            if ((xR & 1) == 1) maxLoan = min(maxLoan, MaxLoan(level, --xR, pt0)); 
            xL >>= 1; xR >>= 1;
        }
        if constexpr (false)
        {
            int maxLoan2 = MaxA - MinA + 1;
            for (int x = s; x < s + k; ++x) {
                Point delta = pts[0][x] - pts[0][s - 1];
                maxLoan2 = min(maxLoan2, int(delta.y < 0 ? -((-delta.y + delta.x - 1) / delta.x) : delta.y / delta.x)); }
            Assert(maxLoan2 == maxLoan);
        }
        return maxLoan;
    }
};

void Test(int seed, int minN, int maxN, int minA, int maxA, int nQueries = -1)
{
    mt19937_64 r(123 + seed);
    int n = uniform_int_distribution<int>(minN, maxN)(r);
    vector<int> dailyIncome(n, 0);
    for (auto &a : dailyIncome) a = uniform_int_distribution<int>(minA, maxA)(r);
    Solver solver(dailyIncome);
    if (nQueries < 0) 
        for (int s = 1; s <= n; ++s) for (int k = 1; s + k - 1 <= n; ++k) solver.MaxLoan(s, k);
    else
        while (nQueries-- > 0) {
            int x1 = uniform_int_distribution<int>(1, n)(r);
            int x2 = uniform_int_distribution<int>(1, n)(r);
            int s = min(x1, x2); int k = max(x1, x2) - s + 1;
            solver.MaxLoan(s, k); }
}

int Test()
{
    for (int seed = 0; ; ++seed)
    {
        printf("%d    \r", seed); fflush(stdout);
        Test(seed, 1, 20, 1, 10, -1);
        Test(seed, 1, 20, -10, 10, -1);
        Test(seed, 1, 20, 1, 1000, -1);
        Test(seed, 1, 20, -1000, 1000, -1);
        Test(seed, 50, 100, 1, 1000, -1);
        Test(seed, 50, 100, -1000, 1000, -1);
    }
    return 0;
}

void MaxLoanTest(const Point &T, const vector<Point> &CH)
{
    const int d = CH.size(); 
    int L = 0, R = d - 1;
    while (L < R)
    {
        // Points 0..L-1 lie above the line from T to L.
        // Points R+1..d-1 lie above the line from T to R.
        int M = (L + R) / 2;
        Assert(M < d - 1);
        if (/*M == d - 1 ||*/ Ccw(T, CH[M], CH[M + 1]) > 0) 
            // M+1 is above the line from T to M.  Hence the points M+1..R are above that line too.
            R = M;
        else 
            // M is above the line from T to M+1.  Hence the points L..M-1 are above that line too.
            L = M + 1;
    }
    for (const auto& P : CH)
        Assert(Ccw(T, CH[L], P) >= 0);
}

int MaxLoanTest()
{
    for (int nIter = 0; ; ++nIter)
    {
        if (nIter % 1000 == 0) fprintf(stderr, "%d    \r", nIter);
        mt19937_64 r(123 + nIter);
        int n = uniform_int_distribution<int>(1, 30)(r);
        n = (nIter % 30) + 1;
        vector<int> ddy(n); int dySum = 0;
        for (auto &v : ddy) dySum += v = uniform_int_distribution<int>(1, 3)(r);
        vector<Point> CH(n); 
        for (int Y = 0, DY = -dySum / 2, i = 0; i < n; ++i)
            { CH[i].x = i + 1; CH[i].y = Y; Y += DY; DY += ddy[i]; }
        llint yMin = CH[0].y, yMax = CH[0].y; for (const auto &T : CH) yMin = min(yMin, T.y), yMax = max(yMax, T.y);
        Point T { 0, uniform_int_distribution<int>(yMin - (yMax - yMin) / 3, yMin)(r) };
        MaxLoanTest(T, CH);
    }
    return 0;
}

int main()
{
    //return MaxLoanTest();
    //return Test();
    //freopen("mortgage99.in", "rt", stdin);
    //freopen("mortgage-tests\\test00.in", "rt", stdin);
    //freopen("mortgage-anti64\\mortgage-anti64-01.in", "rt", stdin);
    //freopen("mortgage-anti64\\n400k\\mortgage-anti64-01.in", "rt", stdin);
    int n, m;
    // Read the input data.
    int ok_ = scanf("%d %d", &n, &m); Assert(ok_ == 2);
    Assert(1 <= n); Assert(n <= MaxN); Assert(1 <= m); Assert(m <= MaxM);
    vector<int> monthlyIncome(n, 0);
    for (auto &a : monthlyIncome) {
        ok_ = scanf("%d", &a); Assert(ok_ == 1);
        Assert(MinA <= a); Assert(a <= MaxA); }
    // Initialize the data structure.
    Solver solver(monthlyIncome);
    // Read and process the queries.
    while (m-- > 0)
    {
        int xL, k; ok_ = scanf("%d %d", &xL, &k); Assert(ok_ == 2);
        Assert(1 <= xL); Assert(xL <= n); Assert(1 <= k); Assert(xL - 1 <= n - k);
        if (int maxLoan = solver.MaxLoan(xL, k); maxLoan >= 0) printf("%d\n", maxLoan);
        else printf("stay with parents\n");
    }
    return 0;
}
