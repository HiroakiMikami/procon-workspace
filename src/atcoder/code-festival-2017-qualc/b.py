N = int(input())
As = [int(x) for x in input().split()]

diff = 1
for A in As:
    if A % 2 == 0:
        diff *= 2
    else:
        diff *= 1
ans = int(pow(3, N)) - diff

print(ans)
