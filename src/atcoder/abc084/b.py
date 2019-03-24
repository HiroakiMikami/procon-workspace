A, B = [int(x) for x in input().split()]
S = input()

ans = True
for i in range(len(S)):
    if i == A:
        if S[i] != "-":
            ans = False
            break
    else:
        if S[i] == "-":
            ans = False
            break

print("Yes" if ans else "No")
