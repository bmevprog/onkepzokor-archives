def solve(n,m,sx,sy,d):
  if abs(1-sx) <=d and abs(n-sx) <= d:
    return -1
  if abs(1-sy) <=d and abs(m-sy) <= d:
    return -1
  if abs(1-sx) <= d and abs(1-sy) <= d:
    return -1
  if abs(n-sx) <= d and abs(m-sy) <= d:
    return -1
  return n+m-2

cases = int(input())
for _ in range(cases):
  n,m,sx,sy,d = map(int, input().split(" "))
  print(solve(n,m,sx,sy,d))
