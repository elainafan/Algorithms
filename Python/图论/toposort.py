from collections import deque
from typing import List  # noqa: UP035


def toposort(n: int, edges: List[List[int]]) -> List[int]:  # noqa: UP006
    ma = [[] for _ in range(n)]
    deg = [0] * n
    for p in edges:
        ma[p[0]].append(p[1])
        deg[p[1]] += 1
    q = deque()
    for i in range(n):
        if deg[i] == 0:
            q.append(i)
    res = []
    while q:
        node = q.popleft()
        res.append(node)
        for p in ma[node]:
            deg[p] -= 1
            if deg[p] == 0:
                q.append(p)
    return res
