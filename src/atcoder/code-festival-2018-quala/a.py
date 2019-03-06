A = int(input())
B = int(input())
C = int(input())
S = int(input())

ans = (S - A - B - C) <= 3

print("Yes" if ans else "No")
