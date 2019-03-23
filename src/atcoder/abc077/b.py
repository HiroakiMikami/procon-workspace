N = int(input())

ans = 0
from math import sqrt
for i in range(int(sqrt(N)), 0, -1):
    if i * i <= N:
        ans = i * i
        break

print(ans)
