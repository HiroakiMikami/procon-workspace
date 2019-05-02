A, B, K = [int(x) for x in input().split()]

for i in range(K):
    if i % 2 == 0:
        if A % 2 == 1:
            A -= 1
        t = A // 2
        A -= t
        B += t
    else:
        if B % 2 == 1:
            B -= 1
        t = B // 2
        A += t
        B -= t
ans = (A, B)

print(" ".join(map(str, ans)))
