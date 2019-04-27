N, M, A, B = [int(x) for x in input().split()]
LRs = [[int(x) for x in input().split()] for _ in range(M)]

n = [B] * N
for L, R in LRs:
    for i in range(L - 1, R):
        n[i] = A
ans = sum(n)

print(ans)
