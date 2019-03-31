N = int(input())
T, A = [int(x) for x in input().split()]
Hs = [int(x) for x in input().split()]

ans = 0
diff = -1
for i, H in enumerate(Hs):
    t = T - H * 0.006
    d = abs(A - t)
    if diff < 0 or d < diff:
        diff = d
        ans = i + 1

print(ans)
