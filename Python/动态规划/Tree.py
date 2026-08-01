import sys
from typing import List, Tuple  # noqa: UP035


def lastMarkedNodes(edges: List[List[int]]) -> List[int]:  # noqa: UP006
    n = len(edges) + 1
    sys.setrecursionlimit(max(sys.getrecursionlimit(), n * 2 + 10))
    ma = [[] for _ in range(n)]
    for p in edges:
        ma[p[0]].append(p[1])
        ma[p[1]].append(p[0])

    tem: List[Tuple[int, int, int, int]] = [  # noqa: UP006
        (0, 0, 0, 0) for _ in range(n)
    ]  # 最长，最长编号，次长，次长编号

    def dfs(x: int, pa: int) -> Tuple[int, int]:  # noqa: UP006
        m1 = m2 = 0
        t1 = t2 = x
        for p in ma[x]:
            if p == pa:
                continue
            a, b = dfs(p, x)
            a += 1
            if a > m1:
                m2, t2 = m1, t1
                m1, t1 = a, b
            elif a > m2:
                m2, t2 = a, b
        tem[x] = (m1, t1, m2, t2)
        return m1, t1

    dfs(0, -1)
    res = [0] * n  # 代表从x往上走，不经过子树的最大长度
    up_id = [0] * n  # 条件同上，节点编号
    res2 = [0] * n  # 最终节点

    def dfs2(x: int, pa: int) -> None:
        if pa == -1:
            res2[x] = tem[x][1]
        else:
            tem2 = tem[pa][0]
            tem3 = tem[pa][0] + 1
            id = tem[pa][1]
            if tem[x][0] + 1 == tem2 and tem[x][1] == tem[pa][1]:
                tem3 = tem[pa][2] + 1
                id = tem[pa][3]
            if res[pa] + 1 > tem3:
                id = up_id[pa]
            up_id[x] = id
            res[x] = max(tem3, res[pa] + 1)
            if tem[x][0] > res[x]:
                id = tem[x][1]
            res2[x] = id

        for p in ma[x]:
            if p != pa:
                dfs2(p, x)

    dfs2(0, -1)
    return res2


# 换根DP求每个节点为根的子树最大深度及其对应叶子


# CF337D 给定多个标记点，求到所有标记点距离都不超过k的点数
def solve() -> None:
    data = iter(map(int, sys.stdin.buffer.read().split()))
    n, m, k = next(data), next(data), next(data)
    sys.setrecursionlimit(max(sys.getrecursionlimit(), n * 2 + 10))
    tem = [0] * n
    for _ in range(m):
        x = next(data)
        tem[x - 1] = 1

    ma = [[] for _ in range(n)]
    for _ in range(n - 1):
        x, y = next(data), next(data)
        ma[x - 1].append(y - 1)
        ma[y - 1].append(x - 1)

    NEG = -(10**9)
    down = [NEG] * n
    up = [NEG] * n
    mx1 = [NEG] * n
    mx2 = [NEG] * n
    id = [-1] * n

    def upd(x: int, val: int, id2: int) -> None:
        if val > mx1[x]:
            mx2[x] = mx1[x]
            mx1[x] = val
            id[x] = id2
        elif val > mx2[x]:
            mx2[x] = val

    def dfs1(x: int, pa: int) -> None:
        if tem[x]:
            upd(x, 0, -1)
        for p in ma[x]:
            if p == pa:
                continue
            dfs1(p, x)
            if down[p] != NEG:
                upd(x, down[p] + 1, p)
        down[x] = mx1[x]

    dfs1(0, -1)

    def dfs2(x: int, pa: int) -> None:
        for p in ma[x]:
            if p == pa:
                continue
            if up[x] != NEG:
                up[p] = max(up[p], up[x] + 1)
            tem2 = mx2[x] if id[x] == p else mx1[x]
            if tem2 != NEG:
                up[p] = max(up[p], tem2 + 1)
            dfs2(p, x)

    dfs2(0, -1)
    ans = 0
    for i in range(n):
        ans += max(down[i], up[i]) <= k
    print(ans)
