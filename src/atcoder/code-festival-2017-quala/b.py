N, M, K = [int(x) for x in input().split()]

def n_black(n, m):
    return (N - n) * m + (M - m) * n

ans = False
for i in range(N + 1):
    for j in range(M + 1):
        if n_black(i, j) == K:
            ans = True

print("Yes" if ans else "No")
