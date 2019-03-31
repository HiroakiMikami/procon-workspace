S = input()

ans = -1
for i in range(len(S) - 2):
    X = int(S[i:i + 3])
    d = abs(X - 753)
    if ans < 0:
        ans = d
    else:
        ans = min(ans, d)

print(ans)
