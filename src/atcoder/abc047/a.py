a, b, c = [ int(x) for x in input().split() ]

m = max(max(a, b), c)
s = a + b + c
ans = m * 2 == s

print("Yes" if ans else "No")
print()
