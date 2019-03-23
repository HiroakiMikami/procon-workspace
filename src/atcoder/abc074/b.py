N = int(input())
K = int(input())
xs = [ int(x) for x in input().split() ]

ans = 0
for x in xs:
    ans += min(abs(x - 0), abs(x - K)) * 2

print(ans)
