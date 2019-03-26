N, M, X = [int(x) for x in input().split()]
As = [int(x) for x in input().split()]

c1 = len(filter(lambda A: A < X, As))
c2 = len(filter(lambda A: A > X, As))
ans = min(c1, c2)

print(ans)
