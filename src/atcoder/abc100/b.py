D, N = [int(x) for x in input().split()]

ans = 1
for _ in range(D):
    ans *= 100
k = 0
for i in range(N):
    k += 1
    if k % 100 == 0:
        k += 1
ans *= k

print(ans)
