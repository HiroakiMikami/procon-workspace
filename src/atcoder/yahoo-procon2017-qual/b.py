N, K = [int(x) for x in input().split()]
As = [int(x) for x in input().split()]

As.sort()
ans = 0
for i in range(K):
    ans += As[i] + i

print(ans)
