A, B, C, D = [ int(x) for x in input().split() ]

if A <= C and A + B >= C:
    ans = (A + B) - C
elif C <= A and C + D >= A:
    ans = (C + D) - A
else:
    ans = 0

print(ans)
