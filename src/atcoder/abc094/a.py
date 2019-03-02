A, B, X = [ int(x) for x in input().split() ]

ans = A <= X and (A + B) >= X

print("YES" if ans else "NO")
print()
