X = int(input())

from math import sqrt
for i in range(1, int(sqrt(X)) + 1):
    if i == 1:
        ans = i
    for b in range(2, int(sqrt(i)) + 1):
        tmp = b * b
        while tmp <= i:
            tmp *= b
        print(tmp, b)
        if tmp == i:
            ans = i

print(ans)
