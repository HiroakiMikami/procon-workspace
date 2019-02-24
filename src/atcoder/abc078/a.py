X, Y = [x for x in input().split() ]

if X == Y:
    ans = "="
elif (X - 'a') > (Y - 'a'):
    ans = ">"
else:
    ans = "<"

print(ans)
print()