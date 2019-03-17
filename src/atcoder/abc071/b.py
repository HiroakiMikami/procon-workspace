S = input()

ans = None
for i in range(26):
    c = chr(i + ord('a'))
    if not c in S:
        ans = c
        break

print("None" if ans is None else ans)
