S = input()

import re
s = "keyence"
ans = s in S
for i in range(1, len(s)):
    s1 = s[:i]
    s2 = s[i:]
    ans = ans or re.match(".*{}.*{}".format(s1, s2), S)

print("YES" if ans else "NO")
