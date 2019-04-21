N = int(input())
a_s = [int(x) for x in input().split()]

ans = 0
for i, a in enumerate(a_s):
    if a_s[a - 1] == (i + 1):
        ans += 1
ans //= 2

print(ans)
