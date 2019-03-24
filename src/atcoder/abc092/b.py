N = int(input())
D, X = [int(x) for x in input().split()]
As = [ int(input()) for _ in range(N)]

ans = X
for A in As:
    day = 0
    while day < D:
        ans += 1
        day += A

print(ans)
