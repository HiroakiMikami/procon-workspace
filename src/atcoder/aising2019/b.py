N = int(input())
A, B = [int(x) for x in input().split()]
Ps = [int(x) for x in input().split()]

Ps.sort()
ans = 0
for i, P in enumerate(Ps):
    n1 = len(list(filter(lambda x: x < P, Ps[:i])))
    n2 = len(list(filter(lambda x: x > P, Ps[i+1:])))
    ans += n1 * n2

print(ans)
