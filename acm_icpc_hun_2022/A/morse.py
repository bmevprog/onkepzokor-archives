from collections import Counter
s = ''.join(filter(str.isalnum, input())).lower()
c = Counter(s).most_common()

n = 21
dp = [0]*n

dp[1] = 1
dp[3] = 2

for i in range(5,n):
    dp[i] = dp[i-2] + dp[i-4]

allength = 0
l = 1
for key, count in c:
    while dp[l] < 1:
        l += 1
    allength += count * l
    dp[l] -= 1

print(allength + (len(s) - 1)*3)

