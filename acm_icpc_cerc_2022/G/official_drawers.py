import sys
n = int(sys.stdin.readline())

# For any n >= 7, the following arrangement of n notebooks and n drawers
# gives the greedy algorithm a 1/2^(n - 6) probability of finding a solution.

nn = n + n
L = [(1, nn - 2)] * 3 + [(3, nn - k) for k in range(3, n - 3)] + [(n - 3, n + 1), (n - 3, n + 1), (n - 2, n + 1)]
assert len(L) == n
for notebook in L: print("%d %d" % notebook)
print()
for w in range(n): print("%d %d" % (w + 1, nn - w))

#print("{%s}," % ", ".join("{%d, %d}" % notebook for notebook in L))
#print("{%s}," % ", ".join("{%d, %d}" % (w + 1, nn - w) for w in range(n)))
