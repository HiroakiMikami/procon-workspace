N = int(input())
As = [int(x) for x in input().split()]

Ss = [As[0]]
nums = {}
nums[As[0]] += 1
nums[0] += 1
for A in As[1:]:
    S = Ss[-1] + A
    Ss.append(S)
    if not S in nums:
        nums[S] = 0
    nums[S] += 1
print(nums)
ans = 0
for n in nums.values():
    ans +=  (n * (n - 1)) // 2

print(ans)
