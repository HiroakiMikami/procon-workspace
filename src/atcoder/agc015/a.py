N, A, B = [int(x) for x in input().split()]

if B > A:
    ans = (N - 1) * B + A - ((N - 1) * A + B) + 1
else:
    ans = 0

print(ans)
