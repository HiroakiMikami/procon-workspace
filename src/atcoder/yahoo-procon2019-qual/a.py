N, K = [ int(x) for x in input().split() ]

ans = 2 * K - 1 <= N

print("YES" if ans else "NO")
