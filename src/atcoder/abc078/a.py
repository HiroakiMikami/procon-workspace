X, Y = [x for x in input().split() ]

if X == Y:
    ans = "="
elif chr(X[0]) > chr(Y[0]):
    ans = ">"
else:
    ans = "<"

print(ans)
print()