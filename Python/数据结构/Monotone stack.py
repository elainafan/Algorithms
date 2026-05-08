from typing import List, Tuple


def nearest_less(nums: List[int]) -> Tuple[List[int], List[int]]:
    n = len(nums)
    l = [-1] * n
    r = [n] * n
    s = []

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
    return l, r


def largestRectangleArea(nums: List[int]) -> int:
    l, r = nearest_less(nums)
    maxx = 0
    for i, x in enumerate(nums):
        maxx = max(maxx, x * (r[i] - l[i] - 1))
    return maxx
