A, B, C, D = [ int(x) for x in input().split() ]

if A <= C and B >= C:
    ans = B - C
elif C <= A and D >= A:
    ans = D - A
else:
    ans = 0

print(ans)
