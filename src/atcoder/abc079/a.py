N = int(input())

xs = [ N // 1000, (N // 100) % 10, (N // 10) % 10, N % 10]
ans = xs[0] == xs[1] == xs[2] or xs[1] == xs[2] == xs[3]

print("Yes" if ans else "No")
print()
