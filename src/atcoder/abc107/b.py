H, W  = [int(x) for x in input().split()]
a_s = [input() for _ in range(H)]

rs = []
for a in a_s:
    if "#" in a:
        rs.append(a)
ans = ["" for _ in rs]
for i in range(W):
    comp = True
    for r in rs:
        if r[i] == "#":
            comp = False
    if not comp:
        for j, r in enumerate(rs):
            ans[j] += r[i]

print("\n".join(ans))
