n, L, R = [int(x) for x in input().split()]
a_s = [int(x) for x in input().split()]

ans = []
for a in a_s:
    ans.append(min(R, max(L, a)))

print(" ".join(map(str, ans)))
