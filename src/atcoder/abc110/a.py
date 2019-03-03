A, B, C = [ int(x) for x in input().split() ]

ans = max(A, B, C) * 10 + (A + B + C - max(A, B, C))

print(ans)
print()
