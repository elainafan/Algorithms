# 拓扑排序

from collections import defaultdict, deque, Counter
from functools import cache
from sortedcontainers import SortedSet, SortedDict, SortedList
from typing import List, Tuple, Set
from math import inf
from heapq import heappop, heappush


def topologicalSort(n: int, edges: List[List[int]]) -> List[int]:
    ma = [[] for _ in range(n)]
    deg = [0] * n
    for x, y in edges:
        ma[x].append(y)
        ma[y].append(x)
    res = []
    q = deque(i for i, d in enumerate(deg) if d == 0)
    while q:
        x = q.popleft()
        res.append(x)
        for y in ma[x]:
            deg[y] -= 1
            if deg[y] == 0:
                q.append(y)
    if len(res) < n:
        return []
    return res
