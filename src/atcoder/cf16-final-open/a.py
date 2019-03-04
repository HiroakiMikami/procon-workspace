H, W = [ int(x) for x in input().split() ]
Ss = [ input().split() for _ in range(H) ]

for i in range(H):
    for j in range(W):
        if Ss[i][j] == "snuke":
            ans = "{}{}".format(chr(j + 'A'), i + 1)

print(ans)
print()
