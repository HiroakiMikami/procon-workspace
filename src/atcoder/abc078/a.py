X, Y = [x for x in input().split() ]

if X == Y:
    ans = "="
elif ord(X[0]) > ord(Y[0]):
    ans = ">"
else:
    ans = "<"

print(ans)
print()