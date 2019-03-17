N, K = [ int(x) for x in input().split() ]
ls = [ int(x) for x in input().split() ]

ls.sort()
ans = sum(ls[-K:])

print(ans)
