H, W = [ int(x) for x in input().split() ]
Ss = [ input() for _ in range(H) ]

ans = Ss.copy()
for y in range(H):
    for x in range(W):
        if ans[y][x] == "#":
            continue
        n = 0
        for dir in [[0, 1], [0, -1], [1, 0], [1, 1], [1, -1], [-1, 0], [-1, 1], [-1, -1]]:
            x2 = x + dir[0]
            y2 = y + dir[1]
            if x2 < 0 or x2 >= W or y2 < 0 or y2 >= H:
                continue
            n += 1 if Ss[y2][x2] == "#" else 0
        ans[y] = ans[y][:x] + str(n) ans[y][x+1:]

print("\n".join(ans))
