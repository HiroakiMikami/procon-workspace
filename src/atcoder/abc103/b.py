S = input()
T = input()

ans = False
for i in range(1, len(S) + 1):
    S_ = S[-i:] + S[:len(S) - i]
    if S_ == T:
        ans = True

print("Yes" if ans else "No")
