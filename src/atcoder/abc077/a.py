Cs = [input(), input()]

ans = Cs[0][0] == Cs[1][2] and Cs[0][1] == Cs[1][1] and Cs[0][2] == Cs[1][0]

print("YES" if ans else "NO")
print()
