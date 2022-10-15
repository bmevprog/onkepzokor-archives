s = sorted(map(int, input().replace('.', '').split(" ")))
targ = int(input().replace('.', ''))

if 3*targ < s[0] + s[1] + s[2]:
    print("impossible")
elif s[1] + s[2] + s[3] <= 3*targ:
    print("infinite")
else:
    res = str(3*targ-s[1]-s[2])
    print(res[:-2] + '.' + res[-2:])

