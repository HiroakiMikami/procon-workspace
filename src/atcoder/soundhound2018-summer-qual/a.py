a, b = [ int(x) for x in input().split() ]

if a + b == 15:
    ans = "+"
elif a * b == 15:
    ans = "*"
else:
    ans = "x"

print(ans)
