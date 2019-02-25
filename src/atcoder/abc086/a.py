a, b = [ int(x) for x in input().split() ]

ans = "Even" if (a * b) % 2 == 0 else "Odd"

print(ans)
print()
