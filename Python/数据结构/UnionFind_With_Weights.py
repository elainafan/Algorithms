class UnionFind:
    def __init__(self, n: int) -> None:
        self.fa = list(range(n))
        self.dis = [0] * n  # 表示x到x所在集合的代表元的距离

    def get(self, x: int) -> int:
        root = x
        path = []
        while self.fa[root] != root:
            path.append(root)
            root = self.fa[root]
        for node in reversed(path):
            fa = self.fa[node]
            self.dis[node] += self.dis[fa]
            self.fa[node] = root
        return root

    def same(self, x: int, y: int) -> bool:
        return self.get(x) == self.get(y)

    # 计算 from - to，需要它们在同一个集合中
    # from 是 Python 关键字，参数名最小避为 from_
    def get_relative_distance(self, from_: int, to: int) -> int:
        self.get(from_)
        self.get(to)
        return self.dis[from_] - self.dis[to]

    # 合并 from 和 to，新增信息 from - to = value
    # 若 from 和 to 不在一个集合，则返回 True，否则返回该信息是否与当前关系一致
    def merge(self, from_: int, to: int, value: int) -> bool:
        x, y = self.get(from_), self.get(to)
        if x == y:
            return self.dis[from_] - self.dis[to] == value
        self.dis[x] = value + self.dis[to] - self.dis[from_]  # 更新代表元之间的距离
        self.fa[x] = y
        return True
