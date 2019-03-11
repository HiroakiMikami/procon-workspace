s = input()

ans = ""
for ch in s:
    if ch == "B":
        if ans != "":
            ans = ans[0:len(ans) - 1]
    else:
        ans = ans + ch

print(ans)
