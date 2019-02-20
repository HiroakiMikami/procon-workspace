A, B = [ int(x) for x in input().split() ]

if A == B:
    ans = "Draw"
elif A == 1:
    ans = "Alice"
elif B == 1:
    ans = "Bob"
else:
    ans = "Alice" if A > B else "Bob"

print(ans)
print()
