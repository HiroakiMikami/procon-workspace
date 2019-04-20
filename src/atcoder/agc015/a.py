N, A, B = [int(x) for x in input().split()]

if B > A:
    ans = N * (B - A) + 1
else:
    ans = 0

print(ans)
