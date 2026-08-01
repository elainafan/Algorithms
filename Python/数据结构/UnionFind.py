class UnionFind:
    def __init__(self, n: int) -> None:
        self.fa = list(range(n))
        self.siz = [1] * n  # 集合大小
        self.cc = n  # 连通块个数

    def get(self, x: int) -> int:
        root = x
        while self.fa[root] != root:
            root = self.fa[root]
        while self.fa[x] != x:
            fa = self.fa[x]
            self.fa[x] = root
            x = fa
        return root

    def is_same(self, x: int, y: int) -> bool:
        return self.get(x) == self.get(y)

    def merge(self, from_: int, to: int) -> bool:
        x, y = self.get(from_), self.get(to)
        if x == y:
            return False
        self.fa[x] = y
        self.siz[y] += self.siz[x]
        self.cc -= 1
        return True

    # 查询x所在集合大小
    def get_size(self, x: int) -> int:
        return self.siz[self.get(x)]
