S = input()

ans = True
if S[0] != "A":
    ans = False
if S[2:-1].count("C") != 1:
    ans = False
if len([x for x in S if x.islower()]) != len(S) - 2:
    ans = False

print("AC" if ans else "WA")
