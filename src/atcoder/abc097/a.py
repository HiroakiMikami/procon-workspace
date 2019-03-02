a, b, c, d = [ int(x) for x in input().split() ]

ans = (abs(b - a) <= d and abs(b - c)) or abs(c - a)

print("Yes" if ans else "No")
print()
