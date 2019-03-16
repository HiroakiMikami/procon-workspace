N, M = [ int(x) for x in input().split() ]
ab_s = [ [ int(x) for x in input().split() ] for _ in range(N) ]
cds = [ [ int(x) for x in input().split() ] for _ in range(M) ]

ans = []
for i in range(N):
    a, b = ab_s[i]
    r = -1
    d_min = -1
    for j in range(M):
        c, d = cds[j]
        distance = abs(a - c) + abs(b - d)
        if d_min < 0 or d_min > distance:
            r = j
            d_min = distance
    ans.append(r + 1)

print("\n".join(map(str, ans)))
