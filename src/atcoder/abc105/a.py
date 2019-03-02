N, K = [ int(x) for x in input().split() ]

if N % K == 0:
    ans = 0
else:
    ans = 1

print(ans)
print()
