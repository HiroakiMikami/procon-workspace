N, M, K = [int(x) for x in input().split()]
As = [int(x) for x in input().split()]
Bs = [int(x) for x in input().split()]

if N < M:
    ans = "X"
elif N > M:
    ans = "Y"
else:
    ans = "Same"
    for A, B in zip(As, Bs):
        if A < B:
            ans = "X"
            break
        elif A > B:
            ans = "Y"
            break

print(ans)
