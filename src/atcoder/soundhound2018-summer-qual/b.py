S = input()
w = int(input())

ans = ""
for i in range(len(S) // w):
    ans += S[i * w]

print(ans)
