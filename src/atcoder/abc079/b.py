N = int(input())

Ls = [2, 1]
for i in range(2, N + 1):
    L = Ls[i - 1] + Ls[i - 2]
    Ls.append(L)

ans = Ls[-1]

print(ans)
