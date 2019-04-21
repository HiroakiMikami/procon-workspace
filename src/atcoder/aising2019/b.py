N = int(input())
A, B = [int(x) for x in input().split()]
Ps = [int(x) for x in input().split()]

n1 = len([x for x in Ps if x <= A])
n2 = len([x for x in Ps if A < x <= B])
n3 = len([x for x in Ps if B + 1 <= x])
ans = n1 * n2 * n3

print(ans)
