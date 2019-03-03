A, B, K = [ int(x) for x in input().split() ]

cnt = 0
for i in range(1, min(A, B), -1):
    print(i)
    if A % i == 0 and B % i == 0:
        cnt += 1
        if cnt == K:
            ans = i
            break

print(ans)
print()
