N = int(input())
s = input()

ans = s.count("R") > s.count("B")

print("Yes" if ans else "No")
