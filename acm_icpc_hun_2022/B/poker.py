from collections import Counter
print(Counter(map(lambda x: x[0], input().split(" "))).most_common()[0][1])

