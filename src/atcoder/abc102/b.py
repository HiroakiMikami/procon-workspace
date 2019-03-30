N = int(input())
As = [int(x) for x in input().split()]

ans = 0
for i in range(N):
    for j in range(N):
        if i == j:
            continue
        ans = max(abs(As[i] - As[j]), ans)

print(ans)
