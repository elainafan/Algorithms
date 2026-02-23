# 图的广度优先遍历

from collections import defaultdict, deque, Counter
from functools import cache
from sortedcontainers import SortedSet, SortedDict, SortedList
from typing import List, Tuple, Set
from math import inf
from heapq import heappop, heappush


class Solution:
    def minCost(self, n: int, edges: List[List[int]], k: int) -> int:
        ma = [[] for _ in range(n)]
        for x, y, z in edges:
            ma[x].append((y, z))
            ma[y].append((x, z))
        l, r = 0, int(1e9)
        ans = -1

        def check(x) -> bool:
            q = deque([0])
            dis = [-1] * n
            dis[0] = 0
            while q:
                node = q.popleft()
                for y, z in ma[node]:
                    if dis[y] < 0 and z <= x:
                        dis[y] = dis[node] + 1
                        q.append(y)
            return 0 <= dis[n - 1] <= k

        while l <= r:
            mid = (l + r) // 2
            if check(mid):
                ans = mid
                r = mid - 1
            else:
                l = mid + 1
        return ans
