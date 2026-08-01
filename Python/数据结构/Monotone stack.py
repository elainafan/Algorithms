def largestRectangleArea(nums: list[int]) -> int:
    n = len(nums)
    r = [n] * n
    l = [-1] * n
    s: list[int] = []
    for i in range(n - 1, -1, -1):
        while s and nums[s[-1]] >= nums[i]:
            s.pop()
        if s:
            r[i] = s[-1]
        s.append(i)
    # 求右边第一个小于的下标

    s.clear()
    for i in range(n):
        while s and nums[s[-1]] >= nums[i]:
            s.pop()
        if s:
            l[i] = s[-1]
        s.append(i)
    # 求左边第一个小于的下标

    maxx = -(1 << 31)
    for i in range(n):
        maxx = max(maxx, nums[i] * (r[i] - l[i] - 1))
    return maxx
