N, M, C = [ int(x) for x in input().split() ]
Bs = [ int(x) for x in input().split()  ]
As = [ [ int(x) for x in input().split() ] for _ in range(N) ]

ans = 0
for A in As:
    x = C
    for a, b in zip(A, B):
        x += a * b
    if x > 0:
        ans += 1

print(ans)
