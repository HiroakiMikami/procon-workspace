T, A, B, C, D = [ int(x) for x in input().split() ]

if T >= A + C:
    ans = B + D
elif T >= C:
    ans = D
elif T >= A:
    ans = B
else:
    ans = 0

print(ans)
