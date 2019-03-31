N = int(input())
Ls = [int(x) for x in input().split()]

L = max(Ls)
ans = L > (sum(Ls) - L)

print("Yes" if ans else "No")
