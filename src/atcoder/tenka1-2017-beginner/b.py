N = int(input())
ABs = [[int(x) for x in input().split()] for _ in range(N)]

last = sorted(ABs, key=lambda x: x[0])[-1]
ans = last[0] + last[1] - 1

print(ans)
