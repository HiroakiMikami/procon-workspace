N = int(input())

if N % 2 == 0:
    ans = 0
    for i in range(N // 2):
        ans += 4 * i
else:
    ans = 0
    for i in range(N // 2):
        ans += 2 + (i - 1) * 2
    ans += N - 2

print(ans)
