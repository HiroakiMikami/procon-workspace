S = input()

ans = S.count("y") == 1 and S.count("a") == 1 and S.count("h") == 1 and S.count("o") == 2

print("YES" if ans else "NO")
