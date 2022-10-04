for case in range(int(input())):
    m, = ints()
    a = [ints(), ints()]
 
    for y in range(2):
        for x in range(m):
            if a[y][x] == 0:
                a[y][x] = -1
 
    right = [[0] * m, [0] * m]
    right[0][0] = 0
    right[1][0] = max(1, a[1][0] + 1)
    for i in range(1, m):
        if i % 2 == 1:
            right[1][i] = max(a[1][i] + 1, right[1][i - 1] + 1)
            right[0][i] = max(a[0][i] + 1, right[1][i] + 1)
        elif i % 2 == 0:
            right[0][i] = max(a[0][i] + 1, right[0][i - 1] + 1)
            right[1][i] = max(a[1][i] + 1, right[0][i] + 1)
 
    deb(right)
 
    def f(a):
        l = [0] * m
        l[-1] = max(a[0][-1] + 2, a[1][-1] + 1)
        for i in range(1, m):
            x = m - i - 1
            l[x] = max(l[x + 1] + 1, a[0][x] + 2 * i + 2, a[1][x] + 1)
        return l
    
    loop = [
        f(a),
        f([a[1], a[0]])
    ]
 
    deb(loop[0])
    deb(loop[1])
 
    best = 10 ** 10
    for x in range(m):
        y = x % 2
 
        t = right[y][x]
        deb(t + 2 * (m - x) - 1, loop[y][x], end=" -> ")
        t = max(t + 2 * (m - x) - 1, loop[y][x])
        deb(t)
        best = min(best, t)
    
    ans(best)