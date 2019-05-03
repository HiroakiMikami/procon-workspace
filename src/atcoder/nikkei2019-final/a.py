N = int(input())
As = [int(x) for x in input().split()]

S = [0]
for A in As:
    S.append(S[-1] + A)
ans = []
for k in range(1, N + 1):
    x = 0
    for i in range(len(S) - k):
        x = max(x, S[i + k] - S[i])
    ans.append(x)

print("\n".join(map(str, ans)))
