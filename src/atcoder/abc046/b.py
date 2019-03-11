N, K = [ int(x) for x in input().split() ]

ans = K * pow(K - 1, N - 1)

print(ans)
