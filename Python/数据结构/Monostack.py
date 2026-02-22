from collections import defaultdict, deque, Counter
from functools import cache
from sortedcontainers import SortedSet, SortedDict, SortedList
from typing import List, Tuple, Set
import heapq
import math


def largestRectangleArea(nums: List[int]) -> int:
    n = len(nums)
    r = [n] * n
    l = [-1] * n  # noqa: E741
    s = []

    # 找右边第一个小于的下标
    for i in range(n - 1, -1, -1):
        while s and nums[s[-1]] >= nums[i]:
            s.pop()
        if s:
            r[i] = s[-1]
        s.append(i)
    s.clear()

    # 找左边第一个小于的下标
    for i in range(n):
        while s and nums[s[-1]] >= nums[i]:
            s.pop()
        if s:
            l[i] = s[-1]
        s.append(i)

    maxx = 0
    for i in range(n):
        maxx = max(maxx, nums[i] * (r[i] - l[i] + 1))
    return maxx
