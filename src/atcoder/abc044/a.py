N = int(input())
K = int(input())
X = int(input())
Y = int(input())

ans = 0
if N <= K:
    ans = N * X
else:
    ans = K * X + (N - K) * Y

print(ans)
print()
