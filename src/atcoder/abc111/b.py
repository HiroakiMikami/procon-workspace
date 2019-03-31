N = int(input())

for i in range(N, 1000):
    xs = set()
    n = i
    while n > 0:
        xs.add(n % 10)
        n //= 10
    if len(xs) == 1:
        ans = i

print(ans)
