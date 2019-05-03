N, M, K = [int(x) for x in input().split()]
As = [int(x) for x in input().split()]
Bs = [int(x) for x in input().split()]

def pow(i, j):
    if j == 0:
        return 1
    if j == 1:
        return i
    if j % 2 == 0:
        t = pow(i, j // 2)
        return t * t
    else:
        return i * pow(i, j - 1)

X = sum(map(lambda x: x[1] * pow(K, N - x[0] - 1), enumerate(As)))
Y = sum(map(lambda x: x[1] * pow(K, M - x[0] - 1), enumerate(Bs)))
ans = "X" if X < Y else "Y" if X > Y else "Same"

print(ans)
