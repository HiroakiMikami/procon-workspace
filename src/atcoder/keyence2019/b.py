S = input()

s = "keyence"
ans = s in S
for i in range(1, len(s)):
    s1 = s[:i]
    s2 = s[i:]
    if S.startswith(s1) and S.endswith(s2):
        ans = True

print("YES" if ans else "NO")
