N = int(input())
A = input()
B = input()
C = input()

ans = 0
for a, b, c in zip(A, B, C):
    xs = set([a, b, c])
    if len(xs) == 1:
        ans += 2
    elif len(xs) == 2:
        ans += 1
    else:
        ans += 0

print(ans)
