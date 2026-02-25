# floya全源最短路

from collections import defaultdict, deque, Counter
from functools import cache
from sortedcontainers import SortedSet, SortedDict, SortedList
from typing import List, Tuple, Set
from math import inf
from heapq import heappop, heappush
from bisect import bisect_left, bisect_right
import math


def shortestPathFloyd(self, n: int, edges: List[List[int]]) -> List[List[int]]:
    ma = [[inf] * n for _ in range(n)]
    for i in range(n):
        ma[i][i] = 0
    for x, y, z in edges:
        ma[x][y] = min(ma[x][y], z)
        ma[y][x] = min(ma[y][x], z)
    for k in range(n):
        for i in range(n):
            if ma[i][k] == inf:
                continue
            for j in range(n):
                ma[i][j] = min(ma[i][j], ma[i][k] + ma[k][j])
    return ma
