a, b = [int(x) for x in input().split()]

x = int("{}{}".format(a, b))
from math import sqrt
y = int(sqrt(x))
ans = y * y == x

print("Yes" if ans else "No")
