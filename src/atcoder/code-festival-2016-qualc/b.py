K, T = [int(x) for x in input().split()]
a_s = [int(x) for x in input().split()]

a_max = max(a_s)
a_others = K - a_max
ans = max(0, a_max - a_others)

print(ans)
