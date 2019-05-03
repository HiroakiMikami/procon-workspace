N = int(input())
a_s = [int(x) for x in input().split()]

mean = sum(a_s) * 1.0 / N
d = min(map(lambda a: abs(a - mean), a_s))
ans = N
for i in range(N):
    if abs(a_s[i] - mean) == d:
        ans = i
        break

print(ans)
