N = int(input())
Vs = [int(x) for x in input().split()]
Cs = [int(x) for x in input().split()]

ans = 0
for V, C in zip(Vs, Cs):
    if V > C:
        ans += V - C

print(ans)
