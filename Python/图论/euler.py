from typing import List, Tuple


def eulerian_path_undirected(n: int, m: int, ma: List[List[Tuple[int, int]]]) -> List[int]:
    # 无向图欧拉回路/欧拉路径
    # ma的第二维是边的编号
    st = 0
    cnt = 0
    for i in range(n - 1, -1, -1):
        if ma[i]:
            if len(ma[i]) & 1:
                st = i
                cnt += 1
            elif cnt == 0:
                st = i
    # 分别处理欧拉回路和欧拉路径的情况
    if cnt not in (0, 2):
        return []

    used = [False] * m
    head = [0] * n
    path = []

    def dfs(x: int) -> None:
        while head[x] < len(ma[x]):
            y, eid = ma[x][head[x]]
            head[x] += 1
            if used[eid]:
                continue
            used[eid] = True
            dfs(y)
        path.append(x)

    dfs(st)
    path.reverse()
    return path if len(path) == m + 1 else []


def eulerian_path_directed(n: int, m: int, ma: List[List[Tuple[int, int]]]) -> List[int]:
    # 有向图欧拉回路/欧拉路径
    # ma的第二维是边的编号
    indeg = [0] * n  # 入度
    for x in range(n):
        for y, _ in ma[x]:
            indeg[y] += 1

    st = -1
    end = -1
    for i in range(n):
        out = len(ma[i])
        if out == indeg[i] + 1:
            if st != -1:
                return []
            st = i
        elif out + 1 == indeg[i]:
            if end != -1:
                return []
            end = i
        elif out != indeg[i]:
            return []

    # 这里是欧拉路径
    if st == -1:
        st = next((i for i in range(n) if ma[i]), 0)

    # 分别处理欧拉回路和欧拉路径的情况
    head = [0] * n
    path = []

    def dfs(x: int) -> None:
        while head[x] < len(ma[x]):
            y, _ = ma[x][head[x]]
            head[x] += 1
            dfs(y)
        path.append(x)

    dfs(st)
    path.reverse()
    return path if len(path) == m + 1 else []
