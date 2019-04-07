N = int(input())
s = input()
t = input()

common = 0
for i in range(min(len(s), len(t))):
    print(s[-i:], t[:i])
    if s[-i:] == t[:i]:
        common = i
ans = len(s + t[common:])

print(ans)
