n = int(input())

ans = 0
for i in range(3):
    if str(n)[i] == "1":
        ans += 9 * pow(10, 2 - i)
    elif str(n)[i] == "9":
        ans += 1 * pow(10, 2 - i)
    else:
        ans += int(str(n)[i]) * pow(10, 2 - i)

print(ans)
print()
