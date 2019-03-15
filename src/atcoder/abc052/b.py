N = int(input())
S = input()

xs = [0]
for s in S:
    if s == "I":
        xs.append(xs[-1] + 1)
    else:
        xs.append(xs[-1] - 1)
ans = max(xs)

print(ans)
