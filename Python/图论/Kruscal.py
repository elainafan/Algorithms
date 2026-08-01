from typing import List  # noqa: UP035


class UnionFind:
    def __init__(self, n: int) -> None:
        self.fa = [0] * n
        self.cc = n
        self.sz = [1] * n
        for i in range(n):
            self.fa[i] = i

    def find(self, x: int) -> int:
        if self.fa[x] == x:
            return x
        self.fa[x] = self.find(self.fa[x])
        return self.fa[x]

    def merge(self, x: int, y: int) -> bool:
        dx, dy = self.find(x), self.find(y)
        if dx == dy:
            return False
        self.fa[dx] = dy
        self.sz[dy] += self.sz[dx]
        self.cc -= 1
        return True

    def get_size(self, x: int) -> int:
        return self.sz[self.find(x)]


def Kruskal(n: int, edges: List[List[int]]) -> int:  # noqa: UP006
    edges = sorted(edges, key=lambda x: x[2])
    u = UnionFind(n)
    ans = 0
    for p in edges:
        if u.merge(p[0], p[1]):
            ans += p[2]
    if u.cc > 1:
        return -1
    return ans
