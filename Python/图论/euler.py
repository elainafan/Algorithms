from typing import List, Tuple  # noqa: UP035


# 无向图欧拉回路/欧拉路径
def eulerianPathOnUndirectedGraph(
    n: int,
    m: int,
    ma: List[List[Tuple[int, int]]],  # noqa: UP006
) -> List[int]:  # noqa: UP006
    # ma的第二维是边的编号
    for i in range(n):
        ma[i].sort(key=lambda a: a[0])
    st, cnt = 0, 0
    for i in range(n - 1, -1, -1):
        if ma[i]:
            if len(ma[i]) % 2:
                st = i
                cnt += 1
            elif cnt == 0:
                st = i
    # 分别处理欧拉回路和欧拉路径的情况
    if cnt > 2:
        return []
    path = []
    vis = [False] * m
    head = [0] * n

    def dfs(v: int) -> None:
        while head[v] < len(ma[v]):
            to, id = ma[v][head[v]]
            head[v] += 1
            if vis[id]:
                continue
            vis[id] = True
            dfs(to)
        path.append(v)

    dfs(st)
    path.reverse()
    return path


# 有向图欧拉回路/欧拉路径
def eulerianPathOnDirectedGraph(
    n: int,
    m: int,
    ma: List[List[Tuple[int, int]]],  # noqa: UP006
) -> List[int]:  # noqa: UP006
    # ma的第二维是边的编号
    in_ = [0] * n  # 入度
    for i in range(n):
        for p in ma[i]:
            in_[p[0]] += 1
    for i in range(n):
        ma[i].sort(key=lambda a: a[0])
    st, end = -1, -1
    for i in range(n):
        out = len(ma[i])
        if out == in_[i] + 1:
            if st >= 0:
                return []
            st = i
        if out + 1 == in_[i]:
            if end >= 0:
                return []
            end = i
    # 这里是欧拉路径
    if st < 0:
        st = 0
        for i in range(n):
            if ma[i]:
                st = i
                break
    # 分别处理欧拉回路和欧拉路径的情况
    path = []
    head = [0] * n

    def dfs(v: int) -> None:
        while head[v] < len(ma[v]):
            to, _ = ma[v][head[v]]
            head[v] += 1
            dfs(to)
        path.append(v)

    dfs(st)
    path.reverse()
    return path
