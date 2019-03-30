N = int(input())

ans = 0
for i in range(1, N + 1, 2):
    n = 0
    for j in range(1, i + 1):
        if i % j == 0:
            n += 1
    if n == 8:
        ans += 1

print(ans)
