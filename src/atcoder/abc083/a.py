A, B, C, D = [ int(x) for x in input().split() ]

if A + B < C + D:
    ans = "Right"
elif A + B > C + D:
    ans = "Left"
else:
    ans = "Balanced"

print(ans)
print()
