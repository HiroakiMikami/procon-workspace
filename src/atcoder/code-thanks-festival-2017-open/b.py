S = input()

ans = len(S) - 1
for i in range(1, len(S)):
    former = S[:i]
    former = "".join(list(reversed(former)))
    S_ = S[i:]
    if former.startswith(S_):
        ans = min(ans, 2 * i - len(S))
    S__ = S[i - 1:]
    if former.startswith(S__):
        ans = min(ans, 2 * i - 1 - len(S))

print(ans)
