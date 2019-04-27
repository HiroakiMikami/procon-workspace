N, X = [int(x) for x in input().split()]
abs = [[int(x) for x in input().split()] for _ in range(N)]

sorted(abs, key=lambda x: x[1])
print(abs)