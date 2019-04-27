N, X = [int(x) for x in input().split()]
abs = [[int(x) for x in input().split()] for _ in range(N)]

abs = sorted(abs, key=lambda x: -x[1])
ans = sum(map(lambda x: x[0] * x[1], abs)) + abs[0][1] * X

print(ans)
