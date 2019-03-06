A = int(input())
B = int(input())
C = int(input())
S = int(input())

ans = (S - A - B - C) <= 3 and (A + B + C) <= S

print("Yes" if ans else "No")
