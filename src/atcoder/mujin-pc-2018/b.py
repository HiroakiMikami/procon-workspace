A = int(input())
S = input()

ans = False
n = A
for s in S:
    if n == 0:
        ans = True
    if s == "+":
        n += 1
    else:
        n -= 1
if n == 0:
    ans = True

print("Yes" if ans else "No")
