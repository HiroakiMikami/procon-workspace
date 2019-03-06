S = input()

ans = False
for i in range(len(S) - 1):
    if S[i:i+2] == "AC":
        ans = True

print("Yes" if ans else "No")