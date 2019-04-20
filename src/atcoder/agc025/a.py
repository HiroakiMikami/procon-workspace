N = int(input())

ns = []
while (N != 0):
    ns.append(N % 10)
    N //= 10
if sum(ns) == 1:
    ans = 10
else:
    ans = sum(ns)

print(ans)
