H, W = [ int(x) for x in input().split() ]
a_s = [ input() for _ in range(H) ]

ans = ""
for i in range(W + 2):
    ans += "#"
ans += "\n"
for a in a_s:
    ans += "#" + a + "#"
ans += "\n"
for i in range(W + 2):
    ans += "#"

print(ans)
