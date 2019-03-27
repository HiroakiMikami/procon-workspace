N = int(input())
S = input()

ans = 0
for i in range(N + 1):
    X = S[:i]
    Y = S[i:]
    ans = max(ans, intersection(set(X), set(Y)))

print(ans)
