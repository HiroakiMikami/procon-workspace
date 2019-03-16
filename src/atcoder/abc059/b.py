A, B = [ int(x) for x in input().split() ]

if A > B:
    ans = "GREATER"
elif A < B:
    ans = "LESS"
else:
    ans = "EQUAL"

print(ans)
