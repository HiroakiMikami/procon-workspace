A, B, C = [int(x) for x in input().split()]

ans = min(A, B) < C < max(A, B)

print("Yes" if ans else "No")
