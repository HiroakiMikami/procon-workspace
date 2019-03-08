Ns = [ int(x) for x in input().split() ]

d = set(Ns)
ans = d == set([1, 9, 7, 4])

print("YES" if ans else "NO")
