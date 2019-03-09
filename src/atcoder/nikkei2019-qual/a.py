N, A, B = [ int(x) for x in input().split() ]

ans = (min(A, B), max(0, A + B - N))

print(ans[0], ans[1])
