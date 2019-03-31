N, M, X, Y = [int(x) for x in input().split()]
xs = [int(x) for x in input().split()]
ys = [int(x) for x in input().split()]

x = max(X, max(xs))
y = min(Y, min(ys))
ans = x >= y

print("War" if ans else "No War")

