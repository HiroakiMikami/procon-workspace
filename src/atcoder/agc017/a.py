N, P = [int(x) for x in input().split()]
As = [int(x) for x in input().split()]

def combination(n, r):
    ans = 1
    for i in range(n, n - r, -1):
        ans *= i
    for i in range(r):
        ans /= (i + 1)
    return ans
n_e = len(list(filter(lambda x: x % 2 == 0, As)))
n_o = N - n_e

ans = 0
for i in range((1 if P == 1 else 0), n_o + 1, 2):
    ans += int(pow(2, n_e)) * combination(n_o, i)

print(ans)
