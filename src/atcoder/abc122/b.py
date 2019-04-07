S = input()

ans = 0
for i in range(len(S)):
    for j in range(i, len(S) + 1):
        s = set(S[i:j])
        if s <= set(["A", "C", "G", T]):
            ans = max(len(S[i:j]), ans)

print(ans)
