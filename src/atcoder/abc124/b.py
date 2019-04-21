N = int(input())
Hs = [int(x) for x in input().split()]

H_m = 0
ans = 0
for H in Hs:
    ans += 1 if H_m <= H else 0
    H_m = max(H_m, H)

print(ans)
