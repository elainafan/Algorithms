# 图的深度优先遍历

from collections import defaultdict, deque, Counter
from functools import cache
from sortedcontainers import SortedSet, SortedDict, SortedList
from typing import List, Tuple, Set
from math import inf
from heapq import heappop, heappush


class Solution:
    def countComponents(self, n: int, edges: List[List[int]]) -> int:
        ma = [[] for _ in range(n)]
        for x, y in edges:
            ma[x].append(y)
            ma[y].append(x)
        vis = [False] * n

        def dfs(x) -> int:
            vis[x] = True
            size = 1
            for y in ma[x]:
                if not vis[y]:
                    size += dfs(y)
            return size

        ans = 0
        for i in range(n):
            if not vis[i]:
                ans += 1
                dfs(i)
        return ans
