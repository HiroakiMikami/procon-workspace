K, T = [int(x) for x in input().split()]
a_s = [int(x) for x in input().split()]

nums = {}
for a in a_s:
    if not a in nums:
        nums[a] = 0
    nums[a] += 1
a_max = max(nums.values())
a_others = K - a_max
ans = max(0, a_max - a_others)

print(ans)
