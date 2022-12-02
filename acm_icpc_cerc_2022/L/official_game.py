draw = list(map(int, input().split()))[::-1]
table = [[1], [1], [100], [100]]
hand = []
for i in range(98):
    if draw and i % 2 == 0:
        while len(hand) < 8:
            hand.append(draw.pop())
    
    move = None
    score = float('inf')
    for i, p in enumerate(hand):
        for j, t in enumerate(table):
            tmp = float('inf')
            if j < 2 and (p > t[-1] or p == t[-1] - 10):
                tmp = p - t[-1]
            if j >= 2 and (p < t[-1] or p == t[-1] + 10):
                tmp = t[-1] - p
            if tmp < score:
                score = tmp
                move = i, j
    if move is None:
        break
    table[move[1]].append(hand.pop(move[0]))

for r in table:
    print(*r)
print(*hand)
print(*draw[::-1])
     
