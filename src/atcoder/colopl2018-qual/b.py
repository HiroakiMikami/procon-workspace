N, X = [int(x) for x in input().split()]
S = input()
Ts = [int(input()) for _ in range(N)]

ans = 0
for i, s in enumerate(S):
    if s == 0:
        ans += Ts[i]
    else:
        ans += min(X, Ts[i])

print(ans)
