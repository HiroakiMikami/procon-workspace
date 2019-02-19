A, op, B = [ x for x in input().split() ]
A = int(A)
B = int(B)

ans = 0
if op == "+":
    ans = A + B
else:
    ans = A - B

print(ans)
print()
