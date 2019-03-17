N = int(input())
a_s = [ int(input()) for _ in range(N) ]

b = 1
ans = None
for i in range(N):
    b = a_s[b - 1]
    if b == 2:
        ans = i + 1

print(-1 if ans is None else ans)