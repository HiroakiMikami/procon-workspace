S = input()

for i in range(len(S)):
    former = S[:i]
    former = reversed(former)
    S_ = S[i:]
    if former.startWith(S_):
        ans = 2 * i - len(S)
        break

print(ans)
