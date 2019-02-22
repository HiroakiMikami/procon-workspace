A, B, C = [ x for x in input().split() ]

ans = A[-1] == B[0] and B[-1] == C[0]

print("YES" if ans else "NO")
print()
