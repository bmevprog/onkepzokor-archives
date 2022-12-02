# Jure Slak, Greedy Drawers, CERC 2022
def counter(a, b, n, k):
    """
    Constructs a counterexample with probability of success equal to 1/(k+1).
    Example has 2n+1 notebooks.
    """
    assert a >= 0
    assert a+4 < b+1
    assert k <= n-2
    assert k >= 1

    notebooks = [(a+1, b+3)]*n + [(a+2, b+2)] + [(a+3, b+1)]*k + [(a+4, b+1)]*(n-k)
    drawers = [(a+1, b+3)]*(n-k) + [(a+2, b+3)]*k + [(a+3, b+2)] + [(a+4, b+1)]*n
    return notebooks, drawers

N = int(input())
assert 150 <= N <= 250, N
rep = N // 7

nb = []
dr = []
for i in range(1, rep+1):
    # Chance of greedy succeeding in 1 rep: 0.5
    n, d = counter(500-5*i, 500+5*i, 3, 1)
    nb.extend(n)
    dr.extend(d)

while len(nb) < N:
    nb.append((1, 100))
    dr.append((1, 100))

# Chance of greedy succeeding in rep reps: 0.5^rep = 0.5^21 = 0.48e-6
# Chance of OK result in 20 trials: (1-0.5^rep)^20 = 1 - 0.95e-5
print("\n".join("{} {}".format(*x) for x in nb), end='\n\n')
print("\n".join("{} {}".format(*x) for x in dr))
