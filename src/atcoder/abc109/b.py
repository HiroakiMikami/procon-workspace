N = int(input())
Ws = [input() for _ in range(N)]

ans = True
w = set([Ws[0]])
for i in range(1, N):
    if Ws[i - 1][-1] != Ws[i][0]:
        ans = False
        break
    if Ws[i] in w:
        ans = False
        break
    w.add(Ws[i])

print("Yes" if ans else "No")
