N, K = [ int(x) for x in input().split() ]

ans = K * pow(K, N - 1)

print(ans)
