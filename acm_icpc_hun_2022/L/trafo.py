f, t = map(int, input().split(" "))

ops = 0
while t < f:
    ops += 1 + f%2
    f = f // 2 + f % 2

print(ops + t-f)
