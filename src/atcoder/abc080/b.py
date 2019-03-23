N = int(input())

f_x = 0
while N > 0:
    f_x += N % 10
    N //= 10
ans = (N % f_x) == 0
print(f_x)

print("Yes" if ans else "No")
