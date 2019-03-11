def f(x):
    return (x * x + f) // 8.0
ans = f(f(f(20)))

print(ans)
