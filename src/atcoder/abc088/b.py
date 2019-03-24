N = int(input())
a_s = [int(x) for x in input().split()]

a_s = a_s.sort()
alice = 0
bob = 0
for i, a in enumerate(a_s.reverse()):
    if i % 2 == 0:
        alice += a
    else:
        bob += a
ans = alice - bob

print(ans)
