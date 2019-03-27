X = int(input())

from math import sqrt
for i in range(1, int(sqrt(X))):
    for b in range(1, int(sqrt(i))):
        tmp = b * b
        while tmp < i:
            tmp *= b
        if tmp == i:
            ans = i
            break

print(ans)
