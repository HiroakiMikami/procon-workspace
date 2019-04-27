X, Y = [int(x) for x in input().split()]

x = 3 * X - Y
if x < 0 or x % 8 != 0:
    ans = False
else:
    x //= 8
    a = X - 3 * x
    if a < 0:
        ans = False
    else:
        ans = True

print("Yes" if ans else "No")
