x1, y1, x2, y2 = [int(x) for x in input().split()]

v = [x2 - x1, y2 - y1]
v_rot = [-v[1],  v[0]]
x3, y3 = [x2 + v_rot[0], y2 + v_rot[1]]
x4, y4 = [x1 + v_rot[0], y1 + v_rot[1]]
ans = [x3, y3, x4, y4]

print(" ".join(map(str, ans)))
