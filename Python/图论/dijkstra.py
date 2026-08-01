from heapq import heappop, heappush
from typing import List  # noqa: UP035


def dij(n: int, edges: List[List[int]], st: int) -> List[int]:  # noqa: UP006
    ma = [[] for _ in range(n)]
    for p in edges:
        ma[p[0]].append((p[1], p[2]))
        ma[p[1]].append((p[0], p[2]))
    dis = [(1 << 63) - 1] * n
    q = []
    dis[st] = 0
    heappush(q, (0, st))
    while q:
        dis_x, x = heappop(q)
        if dis_x > dis[x]:
            continue
        for y, z in ma[x]:
            new_y = dis_x + z
            if new_y < dis[y]:
                dis[y] = new_y
                heappush(q, (new_y, y))
    return dis
