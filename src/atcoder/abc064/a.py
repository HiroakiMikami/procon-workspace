a, b, c = [ int(x) for x in input().split() ]

x = 10 * b + c
ans = x % 4 == 0

print("YES" if ans else "NO")
print()
