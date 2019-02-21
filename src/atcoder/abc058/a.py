a, b, c = [ int(x) for x in input().split() ]

ans =  (b - a) == (c - b)

print("YES" if ans else "NO")
print()
