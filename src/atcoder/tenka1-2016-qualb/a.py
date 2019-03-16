def f(x):
    return int((x * x + 4) // 8.0)
ans = f(f(f(20)))

print(ans)
