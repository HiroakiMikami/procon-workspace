s = input()
t = input()

def histgram(s):
    h = {}
    for ch in s:
        if not ch in h:
            h[ch] = 0
        h[ch] += 1
    return h
h_s = histgram(s)
h_t = histgram(t)
s_ = ""
for i in range(26):
    ch = chr(i + ord('a'))
    for j in range(h_s[ch]):
        s_ += ch
t_ = ""
for i in range(26, 0, -1):
    ch = chr(i + ord('a'))
    for j in range(h_t[ch]):
        t_ += ch
ans = s_ < t_

print("Yes" if ans else "No")