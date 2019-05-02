N = int(input())
S = input()
K = int(input())

c = S[K]
ans = ""
for s in S:
    if s == c:
        ans += s
    else:
        ans += "*"

print(ans)
