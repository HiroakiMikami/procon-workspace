H, W = [ int(x) for x in input().split() ]
h, w = [ int(x) for x in input().split() ]

ans = H * W - (w * H + h * W - h * w)

print(ans)
