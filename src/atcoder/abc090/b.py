A, B = [int(x) for x in input().split()]

ans = 0
for i in range(A, B + 1):
    xs = []
    x = i
    while x > 0:
        xs.append(x % 10)
        x //= 10
    if xs == list(reversed(xs)):
        ans += 1

print(ans)
