x, y = [ int(x) for x in input().split() ]

def group(x):
    g = {
        0: set([1, 3, 5, 7, 8, 10, 12]),
        1: set([4, 6, 9, 11]),
        2: set([2])
    }
    for k in g.keys():
        if x in g[k]:
            return k
    return len(g)
ans = group(x) == group(y)

print("Yes" if ans else "No")
print()
