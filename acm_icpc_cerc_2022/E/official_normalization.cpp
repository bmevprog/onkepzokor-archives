#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <queue>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cassert>
#include <random>
#include <algorithm>
#include <utility>
#include <chrono>
#include <unordered_map>
using namespace std;

//#define Assert assert
#define Assert(x) 

constexpr int MaxN = 10'000, MaxA = 10'000;

typedef long long int llint;
typedef double Flt;

Flt Error(const vector<int> &a, vector<Flt> &x)
{
    llint sum = 0; for (llint ai : a) sum += ai * ai;
    Flt d = sqrt(Flt(sum));
    int n = a.size(); if (x.size() != n) x.resize(n);
    int i = 0, k = 0;
    for (int j = 0; j < n; ++j) { x[j] = a[j] / d; if (x[j] < x[i]) i = j; else if (x[j] > x[k]) k = j; }
    Flt maxErr = 0;
    for (int j = 0; j < n; ++j) { Flt aj = a[i] * x[j] / x[i];
        Flt err = abs(aj - a[j]); maxErr = max(maxErr, err); }
    return maxErr;
}

int ErrorTest()
{
    printf("sizeof(Flt) = %d, eps = %g\n", int(sizeof(Flt)), double(numeric_limits<Flt>::epsilon()));
    Flt maxErr = 0; int n = MaxN;
    vector<int> a(n); vector<Flt> x(n);
    for (int i = 0; i < n; ++i) a[i] = (i == 0) ? 1 : MaxA;
    maxErr = max(maxErr, Error(a, x)); printf("%g\n", (double) maxErr);
    for (int nIter = 280000; ; ++nIter)
    {
        mt19937_64 r(123 + nIter);
        for (int i = 0; i < n; ++i) a[i] = uniform_int_distribution<int>(0, MaxA - 1)(r);
        Flt err = Error(a, x); if (err > maxErr) { 
            maxErr = err; printf("%d: %.15e    \n", nIter, (double) maxErr);  fflush(stdout);
        } 
        if (nIter % 1000 == 0) { printf("%d %.15e %.15e    \r", nIter, (double) err, (double) maxErr); fflush(stdout); }
    }
    return 0;
}

int main()
{
    //return ErrorTest();
    //freopen("normalization99.in", "rt", stdin);
    //freopen("normalization-tests\\norm05.in", "rt", stdin);
    int n, ok_; ok_ = scanf("%d", &n); Assert(ok_ == 1); Assert(2 <= n); Assert(n <= MaxN);
    vector<long double> X(n); for (auto &x : X) { ok_ = scanf("%Lf", &x); Assert(ok_ == 1); Assert(x > 0); Assert(x < 1); }
    long double xMin = X[0], xMax = X[1]; for (auto &x : X) if (x < xMin) xMin = x; else if (x > xMax) xMax = x;
    vector<int> A(n);
    long double ratio = xMin / xMax;
    const long double eps = 1e-6;
    long double error = 0, maxRoundingErrAbs = 0, maxRoundingErrRel = 0;
    for (int aMax = 1; aMax <= MaxA; ++aMax) 
    {
        long double aMinF = ratio * aMax;
        long double aMinR = round(aMinF); 
        if (aMinR < 1) continue;
        if (abs(aMinR - aMinF) * 2e5 > aMax) continue;
        llint d2 = 0;
        bool ok = true;
        maxRoundingErrAbs = 0; maxRoundingErrRel = 0;
        for (int i = 0; i < n; ++i)
        {
            long double aF = X[i] * aMax / xMax;
            long double aR = round(aF); if (abs(aR - aF) * 2e5 > aMax) { 
                ok = false; break; 
            }
           A[i] = int(aR); d2 += llint(A[i]) * A[i];
           maxRoundingErrAbs = max(maxRoundingErrAbs, abs(aF - aR));
           maxRoundingErrRel = max(maxRoundingErrRel, abs(aF - aR) / xMax);
        }
        if (! ok) continue;
        long double d = sqrt((long double) d2);
        error = 0;
        for (int i = 0; i < n; ++i)
        {
            long double x = A[i] / d;
            error = max(error, abs(x - X[i]));
            if (abs(x - X[i]) > eps) {
                //fprintf(stderr, "Note: aMax = %d (max rounding err = %.15Le) failed secondary check.\n", aMax, maxRoundingErr);
                ok = false; 
                break; 
            }
        }
        if (ok) break;
    }
    fprintf(stderr, "max |x[i] - normalized r[i]| = %.6Le, max first-phase rounding err = %.6Le abs, %.6Le rel\n", error, maxRoundingErrAbs, maxRoundingErrRel);
    for (int i = 0; i < n; ++i) printf("%d\n", A[i]); return 0;
}