
N = int(input())

from operator import add
from functools import reduce
ans = reduce(add, range(N)) + N

print(ans)
print()