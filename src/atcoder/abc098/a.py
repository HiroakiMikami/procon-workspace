A, B = [ int(x) for x in input().split() ]

ans = max(A + B, A - B, A * B)

print(ans)
print()
