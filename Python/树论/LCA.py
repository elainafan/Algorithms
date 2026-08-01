from typing import List  # noqa: UP035


class TreeAncestor:
    # 直接传边数组
    def __init__(self, edges: List[List[int]]) -> None:  # noqa: UP006
        n = len(edges) + 1
        m = n.bit_length()
        ma = [[] for _ in range(n)]  # 这里还是0-based
        for p in edges:
            x, y = p
            ma[x].append(y)
            ma[y].append(x)
        self.depth = [0] * n  # 深度
        self.pa = [[-1] * m for _ in range(n)]  # 2^i祖先

        def dfs(x: int, fa: int) -> None:
            self.pa[x][0] = fa
            for y in ma[x]:
                if y == fa:
                    continue
                self.depth[y] = self.depth[x] + 1
                dfs(y, x)

        # 预处理深度
        dfs(0, -1)
        for i in range(m - 1):
            for x in range(n):
                if self.pa[x][i] == -1:
                    continue
                self.pa[x][i + 1] = self.pa[self.pa[x][i]][i]
        # 预处理2^i祖先

    def get_depth(self, x: int) -> int:
        return self.depth[x]  # 获取某个点的深度

    def get_kth_ancestor(self, node: int, k: int) -> int:
        x = k
        while node != -1 and x > 0:
            node = self.pa[node][(x & -x).bit_length() - 1]
            x -= x & -x
        return node  # 得到第k个祖先，类似树状数组的倍增

    def get_lca(self, x: int, y: int) -> int:
        if self.depth[x] > self.depth[y]:
            x, y = y, x
        y = self.get_kth_ancestor(y, self.depth[y] - self.depth[x])  # 先跳到深度相同
        if x == y:
            return x
        for i in range(len(self.pa[x]) - 1, -1, -1):
            px, py = self.pa[x][i], self.pa[y][i]
            if px != py:
                x, y = px, py
            # 倍增跳
        return self.pa[x][0]  # 得到最近公共祖先


# 简单的树上倍增模板
def solve() -> None:
    n = int(input())
    pa = [0] * (n + 1)
    up = [[0] * 20 for _ in range(n + 1)]
    for i in range(n + 1):
        up[i][0] = pa[i]
    for j in range(19):
        for i in range(1, n + 1):
            mid = up[i][j]
            if mid != 0:
                up[i][j + 1] = up[mid][j]
