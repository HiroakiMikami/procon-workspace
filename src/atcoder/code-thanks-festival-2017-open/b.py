S = input()

for i in range(len(S) + 1):
    former = S[:i]
    former = "".join(list(reversed(former)))
    S_ = S[i:]
    print(former, S_)
    if former.startswith(S_):
        ans = 2 * i - len(S)
        break

print(ans)
