H, W = [ int(x) for x in input().split() ]
Cs = [ input() for _In range(H) ]

ans = ""
for i in range(2 * H):
    for j in range(W):
        ans += Cs[(i + 1)/2][j]
    ans += "\n"

print(ans)
