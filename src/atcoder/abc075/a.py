A, B, C = [ int(x) for x in input().split() ]

if A == B:
    ans = C
elif B == C:
    ans = A
else:
    ans = B

print(ans)
print()
