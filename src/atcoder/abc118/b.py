N, M = [int(x) for x in input().split()]
KAs = [[int(x) for x in input().split()] for _ in range(N)]

s = set()
for i in range(1, M + 1):
    s.add(i)
for KA in KAs:
    As = KA[1:]
    for i in range(1, M + 1):
        if not i in As and i in s:
            s.remove(i)
ans = len(s)

print(ans)
