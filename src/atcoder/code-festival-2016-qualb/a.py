S = input()

ans = 0
for s1, s2 in zip(S, "CODEFESTIVAL2016"):
    if s1 != s2:
        ans += 1

print(ans)
print()
