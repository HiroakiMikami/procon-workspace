a, b, x = [ int(x) for x in input().split() ]

ans = int(b // x) - int((a - 1) // x)

print(ans)
