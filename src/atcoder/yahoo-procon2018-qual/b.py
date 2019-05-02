X, K = [int(x) for x in input().split()]

unit = 1
for _ in range(K):
    unit *= 10
ans = (X // unit + 1) * unit

print(ans)
