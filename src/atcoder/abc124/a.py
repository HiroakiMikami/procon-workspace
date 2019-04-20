A, B = [ int(x) for x in input().split() ]

ans = max(A, B) + max(min(A, B), max(A, B) - 1)

print(ans)
