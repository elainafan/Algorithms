from typing import List


class TreeAncestor:
    def __init__(self, edges: List[List[int]], root: int = 0):
        # 直接传边数组
        n = len(edges) + 1
        self.log = max(1, n.bit_length())
        self.depth = [0] * n  # 深度
        self.up = [[-1] * self.log for _ in range(n)]  # 2^i祖先
        ma = [[] for _ in range(n)]  # 这里还是0-based
        for p in edges:
            x, y = p
            ma[x].append(y)
            ma[y].append(x)

        stack = [(root, -1)]
        order = []
        while stack:
            x, fa = stack.pop()
            self.up[x][0] = fa
            order.append(x)
            for y in ma[x]:
                if y == fa:
                    continue
                self.depth[y] = self.depth[x] + 1
                stack.append((y, x))
        # 预处理深度

        for j in range(self.log - 1):
            for x in order:
                p = self.up[x][j]
                if p != -1:
                    self.up[x][j + 1] = self.up[p][j]
        # 预处理2^i祖先

    def get_depth(self, x: int) -> int:
        # 获取某个点的深度
        return self.depth[x]

    def get_kth_ancestor(self, x: int, k: int) -> int:
        j = 0
        while x != -1 and k:
            if k & 1:
                x = self.up[x][j]
            k >>= 1
            j += 1
        return x  # 得到第k个祖先，类似树状数组的倍增

    def get_lca(self, x: int, y: int) -> int:
        if self.depth[x] > self.depth[y]:
            x, y = y, x
        y = self.get_kth_ancestor(y, self.depth[y] - self.depth[x])  # 先跳到深度相同
        if x == y:
            return x
        for j in range(self.log - 1, -1, -1):
            if self.up[x][j] != self.up[y][j]:
                x = self.up[x][j]
                y = self.up[y][j]
                # 倍增跳
        return self.up[x][0]  # 得到最近公共祖先


# 简单的树上倍增模板
