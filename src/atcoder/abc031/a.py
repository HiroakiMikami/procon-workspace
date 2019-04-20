N = int(input())
S = input()

nums = {}
for s in S:
    if not s in nums:
        nums[s] = 0
    nums[s] += 1
ans = 1
for s in S:
    ans *= (nums[s] + 1)
    ans %= 1e9 + 7
ans = (ans + 1e9 + 7 - 1) % (1e9 + 7)

print(ans)
