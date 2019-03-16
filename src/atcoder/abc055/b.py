N = int(input())

ans = 1
MOD = 10**9 + 7
for i in range(N):
    ans *= (i + 1)
    ans %= MOD

print(ans)
