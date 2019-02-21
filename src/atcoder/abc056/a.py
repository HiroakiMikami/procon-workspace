a, b = [ x for x in input().split() ]

if a == 'H':
    ans = b
else:
    ans = "D" if b == "H" else "H"

print(ans)
print()
