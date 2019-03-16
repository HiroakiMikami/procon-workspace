A, B, C = [ int(x) for x in input().split() ]

ans = False
for i in range(1, B + 1):
    if (i * A) % B == C:
        ans = True

print("YES" if ans else "NO")
