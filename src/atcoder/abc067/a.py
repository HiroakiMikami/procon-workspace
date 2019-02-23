A, B  = [ int(x) for x in input().split() ]

ans = A % 3 == 0 or B % 3  == 0 or (A + B) % 3 == 0

print("Possible" if ans else "Impossible")
print()

    