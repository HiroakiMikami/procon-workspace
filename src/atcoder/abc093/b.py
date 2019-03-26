A, B, K = [int(x) for x in input().split()]

t1 = min(B, A + K - 1)
t2 = max(A + K, B - K + 1)
ans = []
for i in range(A, t1 + 1):
    ans.append(i)
for i in range(t2, B + 1):
    ans.append(i)

print("\n".join(map(str, ans)))
