A = int(input())
B = int(input())

ans = 0
for i in range(1, A + 1):
    if i % B:
        ans = i

print(ans)
