A, B, C, D = [ int(x) for x in input().split() ]

if A <= C:
    if B <= D:
        ans = max(0, B - C)
    else:
        ans = max(0, D - C)
else:
    if B <= D:
        ans = max(0, B - A)
    else:
        ans = max(0, D - A)

print(ans)
