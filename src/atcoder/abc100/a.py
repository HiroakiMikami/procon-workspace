A, B = [ int(x) for x in input().split() ]

ans = max(A, B) <= 8

print("Yay!" if ans else ":(")
print()
