a, b = [ int(x) for x in input().split() ]

x = b - a
ans = 0
for i in range(1, x):
    ans += i
ans -= a

print(ans)
