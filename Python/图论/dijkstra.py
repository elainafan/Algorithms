# Dijkstra单源最短路

from collections import defaultdict, deque, Counter
from functools import cache
from sortedcontainers import SortedSet, SortedDict, SortedList
from typing import List, Tuple, Set
from math import inf
from heapq import heappop, heappush


def shortestPathDijkstra(n: int, edges: List[List[int]], start: int) -> List[int]:
    ma = [[] for _ in range(n)]
    for x, y, z in edges:
        ma[x].append((y, z))
        ma[y].append((x, z))
    dis = [inf] * n
    dis[start] = 0
    h = [(0, start)]
    while h:
        dis_x, x = heappop(h)
        if dis_x > dis[x]:
            continue
        for y, z in ma[x]:
            new_y = dis_x + z
            if new_y < dis[y]:
                dis[y] = new_y
                heappush(h, (new_y, y))
    return dis
