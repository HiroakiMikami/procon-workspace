N = int(input())
ss = [ input() for _ in range(N)]
M = int(input())
ts = [ input() for _ in range(M)]

result = {}
for s in ss:
    if not s in result:
        result[s] = 0
    result[s] += 1
for t in ts:
    if not t in result:
        result[t] = 0
    result[t] -= 1
ans = max(0, max(result.values()))

print(ans)
