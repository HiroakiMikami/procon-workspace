a, b, c, d = [ int(x) for x in input().split() ]

ans = (abs(b - a) <= d and abs(b - c) <= d) or abs(c - a) <= d

print("Yes" if ans else "No")
print()
