S = input()

for i in range(len(S)):
    former = S[:i]
    former = str(reversed(former))
    S_ = S[i:]
    if former.startswith(S_):
        ans = 2 * i - len(S)
        break

print(ans)
