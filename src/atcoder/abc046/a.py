a, b, c = [int(x) for x in input().split()]

X = set()
for x in [a, b, c]:
    X.add(x)
ans = len(X)

print(ans)
print()
