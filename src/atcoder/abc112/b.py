N, T = [ int(x) for x in input().split() ]
cts = [[ int(x) for x in input().split() ] for _ in range(N)]

ans = min(map(lambda ct: ct[0], filter(lambda ct: ct[1] <= T, cts)))

print(ans)
