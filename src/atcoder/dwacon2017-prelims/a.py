n, a, b = [ int(x) for x in input().split() ]

ans = max(0, a + b - n)

print(ans)
