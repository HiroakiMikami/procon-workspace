N, M, K = [int(x) for x in input().split()]
As = [int(x) for x in input().split()]
Bs = [int(x) for x in input().split()]

from math import pow
X = sum(map(lambda i, A: A * pow(K, N - i - 1), enumerate(As)))
Y = sum(map(lambda i, B: A * pow(K, N - i - 1), enumerate(Bs)))
ans = "X" if X < Y else "Y" if X > Y else "Same"

print(ans)
