# 1-1-1-1 -> 1-1-2 -> 1-3 vagy 2-2 -> 4

n = int(input())
for i in range(n):
  c = input()+input()
  print(len(set(c))-1)
