A, B = [ int(x) for x in input().split() ]

ans = ((A * B) % 2) == 1

print("Yes" if ans else "No")
print()