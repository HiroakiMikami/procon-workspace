N = int(input())

ans = False
for i in range(N // 4 + 1):
    for j in range(N // 7 + 1):
        x = 4 * i + 7 * j
        if x == N:
            ans = True

print("Yes" if ans else "No")
