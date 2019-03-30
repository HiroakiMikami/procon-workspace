S = input()
T = input()

ans = False
for i in range(len(S)):
    S_ = S[:-i] + S[:i]
    if S_ == T:
        ans = True

print("Yes" if ans else "No")
