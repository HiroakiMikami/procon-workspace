N = int(input())

ans = N // 10 == 9 or N % 10 == 9

print("Yes" if ans else "No")
print()
