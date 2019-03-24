N, A, B = [ int(x) for x in input().split() ]

ans = 0
for i in range(1, N + 1):
    s = 0
    n = i
    while n > 0:
        s += n % 10
        n //= 10
    if A <= s <= B:
        ans += 1

print(ans)
