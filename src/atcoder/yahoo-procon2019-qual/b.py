abs = [[int(x) for x in input().split()] for _ in range(3)]

ns = {1: 0, 2: 0, 3: 0, 4: 0}
for a, b in abs:
    ns[a] += 1
    ns[b] += 1
ans = max(ns.values()) == 2

print("YES" if ans else "NO")
