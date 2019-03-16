N, M = [ int(x) for x in input().split() ]
ab_s = [ [ int(x) for x in input().split() ] for _ in range(M) ]

ans = []
for i in range(N):
    n = 0
    for j in range(M):
        a, b = ab_s[j]
        if a == (i + 1) or b == (i + 1):
            n += 1
    ans.append(n)

print("\n".join(map(str, ans)))
    
