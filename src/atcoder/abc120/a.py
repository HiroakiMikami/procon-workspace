A, B, C = [ int(x) for x in input().split() ]

if A * C <= B:
    ans = C
else:
    ans = B // A

print(ans)
print()
