x, a, b = [ int(x) for x in input().split() ]

if abs(x - a) < abs(x - b):
    ans = "A"
else:
    ans = "B"

print(ans)
print()
