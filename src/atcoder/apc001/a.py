X, Y = [ int(x) for x in input().split() ]

if X % Y == 0:
    ans = -1
else:
    ans = X * (Y - 1)

print(ans)
print()
