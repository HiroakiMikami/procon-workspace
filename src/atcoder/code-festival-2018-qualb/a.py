N = int(input())

ans = 100
for i in range(1, 100 + 1):
    if i % N == 0:
        ans -= 1

print(ans)
