
N = int(input())

from operator import add
from functools import reduce
ans = reduce(add, range(N))

print(ans)
print()