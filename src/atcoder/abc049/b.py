H, W = [ int(x) for x in input().split() ]
Cs = [ input() for _ in range(H) ]

ans = ""
for i in range(2 * H):
    for j in range(W):
        ans += Cs[i//2][j]
    ans += "\n"

print(ans)
