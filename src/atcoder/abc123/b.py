A = int(input())
B = int(input())
C = int(input())
D = int(input())
E = int(input())

Xs = [A, B, C, D, E]
Xs = sorted(Xs, key = lambda x: x % 10)
ans = 0
for X in Xs[:-1]:
    ans += (X + 9) // 10 * 10
ans += X[-1]

print(ans)
