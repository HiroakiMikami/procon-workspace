N, M = [ int(x) for x in input().split() ]
As = [ input() for _ in range(N) ]
Bs = [ input() for _ in range(M) ]

def contain(i, j):
    for p in range(M):
        if As[i + p][j:j+M] != Bs[p]:
            return False
    return True
ans = False
for i in range(N - M + 1):
    for j in range(N - M + 1):
        if contain(i, j):
            ans = True

print("Yes" if ans else "No")
