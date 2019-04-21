N, A, B = [int(x) for x in input().split()]
S = input()

ans = []
num_passed = 0
num_foreign = 0
for ch in S:
    if ch == "a":
        ans.append(num_passed < A + B)
    elif ch == "b":
        num_foreign += 1
        ans.append((num_passed < A + B) and (num_foreign <= B))
    else:
        ans.append(False)

    num_passed += 1 if ans[-1] else 0

print("\n".join(lambda x: "Yes" if x else "No"), ans)
