a, b, c = [ int(x) for x in input.split() ]

ans = a + b + c - max(a, b, c)

print(ans)
print()
