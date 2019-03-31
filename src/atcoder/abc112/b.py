N, T = [ int(x) for x in input().split() ]
cts = [[ int(x) for x in input().split() ] for _ in range(N)]

cands = list(map(lambda ct: ct[0], filter(lambda ct: ct[1] <= T, cts)))
if len(cands) == 0:
    ans = "TLE"
else:
    ans = min(cands)

print(ans)
