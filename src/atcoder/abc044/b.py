w = input()

occ = {}
for ch in w:
    if not ch in occ:
        occ[ch] = 0
    occ[ch] += 1
ans = True
for v in occ.values():
    if v % 2 == 1:
        ans = False

print("Yes" if ans else "No")
