A, B, C = [ int(x) for x in input().split() ]

ans = A <= C and C <= B

print("YES" if ans else "NO")
print()
