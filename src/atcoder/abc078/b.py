X, Y, Z = [ int(x) for x in input().split() ]

ans = 0
for i in range(1, X + 1):
    if (i + 1) * Z + i * Y <= X:
        ans = i

print(ans)
