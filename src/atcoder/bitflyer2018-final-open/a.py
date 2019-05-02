N = int(input())
ps = [int(input()) for _ in range(N)]

ans = 0
while True:
    if all(p % 10 == 0 and p != 0 for p in ps):
        ans += 1
        ps = [p // 10 for p in ps]
    else:
        break

print(ans)
