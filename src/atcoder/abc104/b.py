S = input()

ans = True
if S[0] != "A":
    ans = False
print(S[2:-2])
if S[2:-2].count("C") != 1:
    ans = False
if len([x for x in S if x.islower()]) != len(S) - 2:
    ans = False

print("AC" if ans else "WA")
