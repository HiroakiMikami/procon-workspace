N, M, K = [int(x) for x in input().split()]
As = [int(x) for x in input().split()]
Bs = [int(x) for x in input().split()]

X = 0
for i, A in enumerate(As):
    X *= K
    X += A
Y = sum(map(lambda x: x[1] * pow(K, M - x[0] - 1), enumerate(Bs)))
ans = "X" if X < Y else "Y" if X > Y else "Same"

print(ans)
