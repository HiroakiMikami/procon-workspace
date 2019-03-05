s = input()

x = ""
for ch in s:
    if x == "" and ch == "C":
        x = "C"
    elif x == "C" and ch == "F":
        x = "CF"
ans = x == "CF"

print("Yes" if ans else "No")
