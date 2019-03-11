W, H, N = [ int(x) for x in input().split() ]
xyas = [ [ int(x) for x in input().split() ] for _ in range(N) ]

area = [ [ 0 for _ in range(W) ] for _ in range(H) ]
for xya in xyas:
    x, y, a = xya
    if a == 1:
        for i in range(x):
            for j in range(H):
                area[j][i] = 1
    elif a == 2:
        for i in range(x, W):
            for j in range(H):
                area[j][i] = 1
    elif a == 3:
        for i in range(W):
            for j in range(y):
                area[j][i] = 1
    else:
        for i in range(W):
            for j in range(y, H):
                area[j][i] = 1
ans = 0
for s in area:
    for t in s:
        if t == 0:
            ans += 1

print(ans)

