A, B, N = [int(x) for x in input().split()]
X = input()

for x in X:
    if x == "S":
        A = max(A - 1, 0)
    elif x == "C":
        B = max(B - 1, 0)
    else:
        if A >= B:
            A = max(A - 1, 0)
        else:
            B = max(B - 1, 0)
ans = (A, B)

print("\n".join(map(str, ans)))
