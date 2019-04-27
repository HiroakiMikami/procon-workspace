X, Y = [int(x) for x in input().split()]

x = 3 * X - Y
ans = x > 0 and x % 8 == 0

print("Yes" if ans else "No")
