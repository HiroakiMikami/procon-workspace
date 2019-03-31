N = int(input())
xus = [[x for x in input().split()] for _ in range(N)]

ans = 0
for x, u in xus:
    if u == "JPY":
        ans += int(x)
    else:
        ans *= 380000.0 * float(x)

print(ans)
