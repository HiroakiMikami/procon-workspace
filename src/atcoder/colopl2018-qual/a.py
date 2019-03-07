A, B = [ int(x) for x in input().split() ]
S = int(input())

ans = len(S) >= A and len(S) <= B

print("YES" if ans else "NO")
