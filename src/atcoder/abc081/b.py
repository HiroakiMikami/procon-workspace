N = int(input())
As = [ int(x) for x in input().split() ]

ans = 0
while True:
    finished = False
    for i, A in enumerate(As):
        if A % 2 == 1:
            finished = True
            break
        As[i] /= 2
    if finished:
        break
    ans += 1

print(ans)
