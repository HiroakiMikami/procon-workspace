A, B, C = [int(x) for x in input().split()]

ans = min(A, C) < B < max(A, C)

print("Yes" if ans else "No")
