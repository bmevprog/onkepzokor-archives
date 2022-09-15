from bisect import bisect_left # std::lower_bound = bisect.bisect_left(a, x, lo=0, hi=len(a))
cases = int(input())

for _ in range(cases):

  n = int(input())

  a = list(map(int, input().split(" ")))
  b = list(map(int, input().split(" ")))

  # Minimumok: a[i] -> b[j<i], hogy b[j]>=a[i].
  for i in range(n):
    j = bisect_left(b, a[i], lo=0, hi=i+1)
    print(b[j]-a[i], end=" ")
  print()

  for i in range(n):
    for k in range(i, n):
      if k == n-1 or b[k] < a[k+1]:
        break
    print(b[k]-a[i], end=" ")
  print()

