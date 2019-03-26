A, B, C = [int(x) for x in input().split()]
K = int(input())

for _ in range(K):
    if max(A, B, C) == A:
        A *= 2
    elif max(A, B, C) == B:
        B *= 2
    else:
        C *= 2
ans = A + B + C

print(ans)
