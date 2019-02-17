A, B, C = [int(i) for i in input().split()]

five = 0
seven = 0
for x in [A, B, C]:
    if x == 5:
        five += 1
    elif x == 7:
        seven += 1

ans = five == 2 and seven == 1

print("YES" if ans else "NO")
print()