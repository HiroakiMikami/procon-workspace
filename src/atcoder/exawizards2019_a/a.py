A, B, C = [int(x) for x in input().split()]

ans = A == B and B == C

print("Yes" if ans else "No")
