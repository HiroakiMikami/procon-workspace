S = input()

o = set()
ans = True
for s in S:
    if s in o:
        ans = False
    o.add(s)

print("yes" if ans else "no")
