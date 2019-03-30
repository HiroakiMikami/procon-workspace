N = int(input())

S_N = 0
n = N
while n >= 0:
    S_N += n % 10
    n //= 10
ans = (N % S_N) == 0

print("Yes" if ans else "No")
