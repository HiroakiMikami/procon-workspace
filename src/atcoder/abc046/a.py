a = int(input())
b = int(input())
c = int(input())

X = set()
for x in [a, b, c]:
    X.add(x)
ans = len(X)

print(ans)
print()
