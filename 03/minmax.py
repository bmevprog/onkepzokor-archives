from bisect import bisect_left # std::lower_bound = bisect.bisect_left(a, x, lo=0, hi=len(a))
cases = int(input())

for _ in range(cases):

  n = int(input())

  a = list(map(int, input().split(" ")))
  b = list(map(int, input().split(" ")))

  # Minimumok: a[i] -> b[j<i], hogy b[j]>=a[i].
  print(" ".join(map(lambda i: str(b[bisect_left(b, a[i], lo=0, hi=i+1)]-a[i]), range(n))))

  # Maximumok
  mx = [0]*n
  k = 0
  for i in range(n):
    k = max(k,i) # Don't restart search
    while k<n-1 and b[k] >= a[k+1]:
      k += 1
    mx[i] = b[k]-a[i]
  print(" ".join(map(str, mx)))
