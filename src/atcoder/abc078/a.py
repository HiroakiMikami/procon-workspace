X, Y = [x for x in input().split() ]

if X == Y:
    ans = "="
elif (X[0] - 'a') > (Y[0] - 'a'):
    ans = ">"
else:
    ans = "<"

print(ans)
print()