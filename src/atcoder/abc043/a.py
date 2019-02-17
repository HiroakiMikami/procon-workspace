
N = int(input())

from operator import add
ans = reduce(add, range(N))

print(ans)
print()