N = int(input())
a_s = [ int(x) for x in input().split() ]

ans = max(a_s) - min(a_s)

print(ans)
