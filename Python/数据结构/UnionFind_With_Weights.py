class WeightedUnionFind:
    def __init__(self, n: int):
        self.fa = list(range(n))
        self.dis = [0] * n  # 表示x到x所在集合的代表元的距离

    def get(self, x: int) -> int:
        if self.fa[x] != x:
            root = self.get(self.fa[x])
            self.dis[x] += self.dis[self.fa[x]]  # 递归更新x到其代表元的距离
            self.fa[x] = root
        return self.fa[x]

    def find(self, x: int) -> int:
        return self.get(x)

    def is_same(self, x: int, y: int) -> bool:
        return self.get(x) == self.get(y)

    def relative_distance(self, from_: int, to: int) -> int:
        # 计算从from到to的相对距离，需要它们在同一个集合中
        self.get(from_)
        self.get(to)
        return self.dis[from_] - self.dis[to]

    def merge(self, from_: int, to: int, value: int) -> bool:
        # 合并from和to，新增信息to-from=value
        # 若to和from不在一个集合，则返回true，否则返回是否与当前信息矛盾
        x = self.get(from_)
        y = self.get(to)
        if x == y:
            return self.dis[from_] - self.dis[to] == value
        self.dis[x] = value + self.dis[to] - self.dis[from_]  # 更新代表元之间的距离
        self.fa[x] = y
        return True
