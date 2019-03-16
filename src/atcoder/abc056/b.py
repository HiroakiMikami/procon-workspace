W, a, b = [ int(x) for x in input().split() ]

if a + W < b:
    ans = b - (a + W)
elif b + W < a:
    ans = a - (b + W)
else:
    ans = 0

print(ans)
