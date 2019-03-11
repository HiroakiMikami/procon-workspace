Ss = {}
Ss["A"] = input()
Ss["B"] = input()
Ss["C"] = input()

p = "A"
while True:
    if len(Ss[p]) == 0:
        ans = p
        break
    ch = Ss[p][0]
    Ss[p] = Ss[p][1:]
    p = ch.toupper()

print(ans)
