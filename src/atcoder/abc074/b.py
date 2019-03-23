N = int(input())
lrs = [ [ int(x) for x in input().split()] for _ in range(N) ]

ans = 0
for lr in lrs:
    print(lr)
    ans += (r - l + 1)

print(ans)
