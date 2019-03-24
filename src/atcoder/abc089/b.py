N = int(input())
Ss = input().split()

ans = "Three" if len(set(Ss)) == 3 else "Four"

print(ans)
