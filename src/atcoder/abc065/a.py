X, A, B = [ int(x) for x in input().split() ]

if B <= A:
    ans = "delicious"
elif B > A + X:
    ans = "dangerous"
else:
    ans = "safe"

print(ans)
print()
