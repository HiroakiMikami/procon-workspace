X = int(input())

from math import sqrt
for i in range(1, X):
    if i == 1:
        ans = i
    print(int(sqrt(i)) + 1, i)
    for b in range(2, int(sqrt(i)) + 1):
        tmp = b * b
        while tmp < i:
            tmp *= b
        print(tmp, b)
        if tmp == i:
            ans = i

print(ans)
