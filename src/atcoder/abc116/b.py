s = int(input())

def f(n):
    if n % 2 == 0:
        return n / 2
    else:
        return 3 * n + 1
a_s = [s]
A = set([s])
for m in range(1, 1000000):
    a_m = f(a_s[-1])
    if a_m in A:
        ans = m + 1
        break

    a_s.append(a_m)
    A.add(a_m)

print(ans)
