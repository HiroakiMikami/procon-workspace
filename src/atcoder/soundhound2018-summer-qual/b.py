S = input()
w = int(input())

ans = ""
for i in range((len(S) + w - 1) // w):
    ans += S[i * w]

print(ans)
