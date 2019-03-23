N = int(input())

f_x = 0
n = N
while n > 0:
    f_x += n % 10
    n //= 10
ans = (N % f_x) == 0

print("Yes" if ans else "No")
