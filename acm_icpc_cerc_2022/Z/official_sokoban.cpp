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

constexpr int MaxN = 150, MinDim = 3, MaxDim = 8, MinBi = 1, MaxBi = 4;
constexpr int Six = MaxDim - 2;

typedef long long int llint;
constexpr llint BoxMask = (1LL << (Six * Six)) - 1;
llint EncodeState(int p, llint boxes) { return (llint(p) << (Six * Six)) | boxes; }

bool ProcessCase()
{
    // Read the initial state.
    constexpr char Wall = '#', Player = '@', PlayerStorage = '+', Box = '$', BoxStorage = '*', Storage = '.', Free = ' ';
    vector<string> a; int w = -1;
    while (true)
    {
        string line; getline(cin, line);
        if (line.empty()) break;
        a.emplace_back(line);
        w = max(w, (int) line.length());
        for (char c : line) Assert(c == Wall || c == Player || c == PlayerStorage || c == Box || c == BoxStorage || c == Storage || c == Free); 
    }
    if (a.empty()) return false;
    const int h = a.size(); Assert(MinDim <= h); Assert(h <= MaxDim);
    // Add trailing spaces where needed.
    Assert(MinDim <= w); Assert(w <= MaxDim);
    for (auto &line : a) while (line.length() < w) line += Free;
    // See which tiles are reachable from the player's initial position.
    constexpr int DX[] = {-1, 1, 0, 0}, DY[] = {0, 0, -1, 1};
    llint verts = 0; int nVerts = 0; stack<int> toDo;
    for (int y = 0; y < h; ++y) for (int x = 0; x < w; ++x) { 
        if (a[y][x] == Player || a[y][x] == PlayerStorage) { 
            Assert(1 <= y); Assert(1 <= x); Assert(x < w - 1); Assert(y < h - 1);
            toDo.emplace((y - 1) * Six + (x - 1)); } }
    Assert(toDo.size() == 1); // only one player
    int playerLocation = toDo.top(); verts |= (1LL << playerLocation); ++nVerts;
    while (! toDo.empty())
    {
        int u = toDo.top(); toDo.pop(); 
        int x = u % Six, y = u / Six;
        for (int dir = 0; dir < 4; ++dir)
        {
            int xx = x + DX[dir], yy = y + DY[dir]; 
            Assert(-1 <= xx); Assert(xx < w - 1); Assert(-1 <= y); Assert(y < h - 1); // the player is supposed to be walled in
            if (a[yy + 1][xx + 1] == Wall) continue;
            Assert(0 <= xx); Assert(xx < w - 2); Assert(0 <= y); Assert(y < h - 2); // the player is supposed to be walled in
            int v = yy * Six + xx; if ((verts >> v) & 1) continue;
            verts |= (1LL << v); toDo.emplace(v); ++nVerts;
        }
    }
    Assert(nVerts <= (MaxDim - 2) * (MaxDim - 2));
    // Find the storage and box locations; make sure they are all reachable from the player's starting position (0).
    llint storageLocations = 0, boxLocations = 0; int nStorage = 0, nBoxes = 0;
    for (int y = 0; y < h; ++y) for (int x = 0; x < w; ++x)
    {
        int c = a[y][x], u = (1 <= y && y < h - 1 && 1 <= x & x < w - 1) ? (y - 1) * Six + (x - 1) : (Six * Six + 1);
        if (c == Player || c == PlayerStorage) Assert(u == playerLocation); 
        // If there is an unreachable box that already stands on a storage spot, we'll just ignore it.
        if (c == BoxStorage && ((verts >> u) & 1) == 0) { ++nStorage; ++nBoxes; continue; }
        if (c == Storage || c == PlayerStorage || c == BoxStorage) { Assert((verts >> u) & 1); storageLocations |= (1LL << u); ++nStorage; }
        if (c == Box || c == BoxStorage) { Assert((verts >> u) & 1); boxLocations |= (1LL << u); ++nBoxes; }
    }
    // Add the initial state to the heap.
    Assert(nStorage == nBoxes); Assert(nBoxes >= MinBi); Assert(nBoxes <= MaxBi);
    llint initialState = EncodeState(playerLocation, boxLocations);
    unordered_map<llint, int> knownSolutions; knownSolutions.emplace(initialState, 0);
    priority_queue<pair<int, llint>, vector<pair<int, llint>>, greater<pair<int, llint>>> heap; heap.emplace(0, initialState);
    int solutionFound = -1, nStates = 0, nMoves = 0;    
    // Explore the state space.
    while (! heap.empty())
    {
        // Take the next state from the heap.  
        auto [sDist, s] = heap.top(); heap.pop(); ++nStates;
        int bestSDist = knownSolutions[s]; Assert(bestSDist <= sDist); if (bestSDist < sDist) continue;
        int p = s >> (Six * Six); llint boxes = s & BoxMask;
        // Are the boxes all on their storage locations?
        if (boxes == storageLocations) { solutionFound = sDist; break; }
        // Explore possible moves, where a move consists of the player walking around a bit
        // and then pushing a box from one tile to a neighbouring tile.
        Assert(toDo.empty()); toDo.emplace(p); llint seen = 1LL << p;
        while (! toDo.empty())
        {
            int u = toDo.top(); toDo.pop(); int ux = u % Six, uy = u / Six;
            for (int dir = 0; dir < 4; ++dir)
            {
                int vx = ux + DX[dir], vy = uy + DY[dir]; 
                if (vx < 0 || vx >= Six || vy < 0 || vy >= Six) continue; // v must be walled
                int v = vy * Six + vx; llint vMask = 1LL << v; 
                if ((verts & vMask) == 0) continue; // v is walled
                if (boxes & vMask)
                {
                    // The player can get from p to u without moving anything, and can then step
                    // from u to its neighbour v, but in this step he would push a box from v to w.
                    int wx = vx + DX[dir], wy = vy + DY[dir]; 
                    if (wx < 0 || wx >= Six || wy < 0 || wy >= Six) continue; // w must be walled
                    int w = wy * Six + wx; llint wMask = 1LL << w;
                    if ((verts & wMask) == 0) continue; // w is walled
                    if (boxes & wMask) continue; // there's another box on w
                    // At this point we know that the move is possible; add the new state to the heap,
                    // unless we already know an equally good or better solution for it.
                    llint newBoxes = (boxes & ~vMask) | wMask;
                    llint newState = EncodeState(v, newBoxes);
                    auto [it, isNew] = knownSolutions.emplace(newState, sDist + 1);
                    if (! isNew) { if (it->second <= sDist + 1) continue; else it->second = sDist + 1; }
                    heap.emplace(sDist + 1, newState); ++nMoves;
                }
                else if ((seen & vMask) == 0) { 
                    // The player can get from p to u without moving anything, and can then step
                    // from u to its neighbour v, also without moving anything.
                    seen |= vMask; toDo.push(v); }
            }
        }
    }
    // It turns out that no test case requires us to expore more than approx. 23 thousand states.
    fprintf(stderr, "w = %d, h = %d, %d boxes: %d states explored, %d moves added to the heap\n", w, h, nBoxes, nStates, nMoves);
    printf("%d\n", solutionFound);
    return true;
}

int main()
{
    //ifstream ifs("sokoban99.in"); cin.rdbuf(ifs.rdbuf());
    //ifstream ifs("sokoban-tests\\sokoban.in"); cin.rdbuf(ifs.rdbuf());
    int nCases = 0;
    while (ProcessCase()) ++nCases;
    Assert(1 <= nCases); Assert(nCases <= MaxN);
    return 0;
}