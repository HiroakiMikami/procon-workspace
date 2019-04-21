N = int(input())
Ds = [int(x) for x in input().split()]
M = int(input())
Ts = [int(x) for x in input().split()]\

ND = {}
for D in Ds:
    if not D in ND:
        ND[D] = 0
    ND[D] += 1
NT = {}
for T in Ts:
    if not T in NT:
        NT[T] = 0
    NT[T] += 0

print(ND)
print(NT)
ans = True
for T in NT.keys():
    if not T in ND:
        ans = False
        break
    if NT[T] > ND[T]:
        ans = False
        break

print("YES" if ans else "NO")
