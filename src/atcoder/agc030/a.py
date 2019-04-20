A, B, C = [int(x) for x in input().split()]

ans = min(C, A + B + 1) + B

print(ans)
