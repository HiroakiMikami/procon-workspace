N, M, K = [int(x) for x in input().split()]
As = [int(x) for x in input().split()]
Bs = [int(x) for x in input().split()]

X = 0
for A in As:
    X *= K
    X += A
Y = 0
for B in Bs:
    Y *= K
    Y += B
ans = "X" if X < Y else "Y" if X > Y else "Same"

print(ans)
