N = int(input())
a_s = [int(x) for x in input().split()]

ans = 0
start = 0
color = None
for i, a in enumerate(a_s):
    if color is None:
        color = a
    else:
        if color != a:
            print(start, i)
            ans += (i - start) // 2
            color = a
            start = i
ans += (N - start) // 2

print(ans)
