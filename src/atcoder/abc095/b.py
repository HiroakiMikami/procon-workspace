N, X = [int(x) for x in input().split()]
ms = [int(input()) for _ in range(N)]

X -= sum(ms)
m = min(ms)
ans = len(ms) + X // m

print(ans)
