N, L = [ int(x) for x in input().split() ]
Ss = [ input() for _ in range(N) ]

Xs = Ss.sort()
ans = "".join(Xs)

print(ans)
