R, N, M = [int(x) for x in input().split()]

def length(i):
    if i < 0 or i > N:
        return 0
    x = i * 2.0 * R / N
    r = abs(R  - x)
    from math import sqrt
    return 2 * sqrt(R * R - r * r)

ans = 0
for i in range(1, N + M):
    print(max(length(i), length(i - M)))
    ans += max(length(i), length(i - M))

print(ans)
