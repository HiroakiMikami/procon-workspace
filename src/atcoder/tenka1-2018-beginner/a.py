S = input()

if len(S) == 3:
    ans = "".join(reversed(S))
else:
    ans = S

print(ans)
