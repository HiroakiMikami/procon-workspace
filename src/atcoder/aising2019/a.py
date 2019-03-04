N = int(input())
H = int(input())
W = int(input())

if N < H or N < W:
    ans = 0
else:
    ans = (N - H + 1) * (N - W + 1)

print(ans)
print()
