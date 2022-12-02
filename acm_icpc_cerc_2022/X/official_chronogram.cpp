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
#include <unordered_map>
#include <queue>
#include <fstream>
using namespace std;

#define Assert assert
//#define Assert(x) 

int main()
{
    string s; getline(cin, s);
    int t; int ok_ = sscanf(s.c_str(), "%d", &t); Assert(ok_ == 1);
    constexpr int MaxT = 10000, MaxLen = 1000;
    Assert(1 <= t); Assert(t <= MaxT);
    while (t-- > 0)
    {
        getline(cin, s); Assert(1 <= s.length()); Assert(s.length() <= MaxLen);
        int year = 0;
        for (int c : s)
        {
            Assert(c == ' ' || ('A' <= c && c <= 'Z'));
            if (c == 'M') year += 1000; 
            else if (c == 'D') year += 500;
            else if (c == 'C') year += 100;
            else if (c == 'L') year += 50;
            else if (c == 'X') year += 10;
            else if (c == 'V') year += 5;
            else if (c == 'I') year += 1;
        }
        cout << year << endl;
    }
    return 0;
}