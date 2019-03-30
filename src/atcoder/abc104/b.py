S = input()

ans = True
if S[0] != "A":
    ans = False
if count(S[2:-2], "C") != 1:
    ans = False
if len([x for x in S if s.islower()]) != len(S) - 2:
    ans = False

print("AC" if ans else "WA")
