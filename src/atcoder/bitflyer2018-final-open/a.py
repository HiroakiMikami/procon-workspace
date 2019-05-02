N = int(input())
ps = [int(input()) for _ in range(N)]

ans = 0
while True:
    print(ans, ps)
    if all(p % 10 == 0 for p in ps):
        ans += 1
        for p in ps:
            p //= 10
    else:
        break

print(ans)
