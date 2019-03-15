N = int(input())
Ts = [ int(x) for x in input().split() ]
M = int(input())
PXs = [ [ int(x) for x in input().split() ] for _ in range(M) ]

ans = []
s = sum(Ts)
for PX in PXs:
    P, X = PX
    ans.append(s - Ts[P - 1] + X)

print("\n".join(ans))
