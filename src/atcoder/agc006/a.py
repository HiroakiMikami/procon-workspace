N = int(input())
s = input()
t = input()

common = 0
for i in range(min(len(s), len(t))):
    if s[-i:] == t[:i]:
        common = i
ans = s + t[i:]

print(ans)
