X = int(input())

for i in range(1, X):
    for b in range(1, i):
        tmp = b * b
        while tmp < i:
            tmp *= b
        if tmp == i:
            ans = i
            break

print(ans)
