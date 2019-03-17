S = input()

ans = ""
for i in range(1, len(S)):
    s = S[0:len(S) - i]
    if len(s) % 2 == 0 and s[0:len(s)//2] == s[len(s)//2:]:
        ans = len(s)
        break

print(ans)
