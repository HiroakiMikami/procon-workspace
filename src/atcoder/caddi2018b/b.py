N, H, W = [int(x) for x in inpout().split()]
ABs = [[int(x) for x in input().split()] for _ in range(N)]

ans = 0
for A, B in ABs:
    if H <= A and W <= B:
        ans += 1

print(ans)
