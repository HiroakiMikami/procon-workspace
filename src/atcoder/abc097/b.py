X = int(input())

from math import sqrt
for i in range(1, int(sqrt(X)) + 1):
    for b in range(1, int(sqrt(i)) + 1):
        tmp = b * b
        while tmp < i:
            tmp *= b
        if tmp == i:
            ans = i
            break

print(ans)
