S = input()

for i in range(len(S)):
    s = S[0:len(S) - i]
    if len(s) % 2 == 0 and s[0:len(S)//2] == s[len(S)//2]:
        ans = s
        break

print(ans)
