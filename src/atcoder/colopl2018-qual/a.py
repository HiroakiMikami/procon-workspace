A, B = [ int(x) for x in input().split() ]
S = input()

ans = len(S) >= A and len(S) <= B

print("YES" if ans else "NO")
