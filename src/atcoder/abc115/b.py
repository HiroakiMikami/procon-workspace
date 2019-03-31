N = int(input())
ps = [int(input()) for _ in range(N)]

ps = ps.sorted()
ps[-1] /= 2
ans = sum(ps)

print(ans)
