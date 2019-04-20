N, x = [int(x) for x in input().split()]
a_s = [int(x) for x in input().split()]

a_s.sort()
ans = 0
X = 0
for a in a_s[:-1]:
    if X + a <= x:
        X += a
        ans += 1
    else:
        break
print(a_s, X, x)
if a_s[-1] == X - x:
    ans += 1

print(ans)
