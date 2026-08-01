from copy import copy


class Tag:
    __slots__ = ("add",)

    def __init__(self, add: int = 0) -> None:
        self.add = add

    def empty(self) -> bool:
        return self.add == 0

    def apply(self, other: "Tag") -> None:
        self.add += other.add  # 先做已有操作，再做 other


class Info:
    __slots__ = ("mx", "sum")

    def __init__(self, sum: int = 0, mx=None) -> None:
        self.sum = sum
        self.mx = sum if mx is None else mx

    def apply(self, tag: Tag, l: int, r: int) -> None:
        self.sum += tag.add * (r - l + 1)
        self.mx += tag.add

    def __add__(self, other: "Info") -> "Info":
        return Info(self.sum + other.sum, max(self.mx, other.mx))

    def __lt__(self, other: "Info") -> bool:
        return self.mx < other.mx  # 线段树二分用，不需要时可删


class LazySegmentTree:
    def __init__(self, a, init_val=None) -> None:
        if isinstance(a, int):
            init_val = Info() if init_val is None else init_val
            a = [copy(init_val) for _ in range(a)]
        self.n = len(a)
        self.info = [Info() for _ in range(4 * self.n)]
        self.tag = [Tag() for _ in range(4 * self.n)]
        self._build(a, 1, 0, self.n - 1)

    def _apply(self, node: int, l: int, r: int, val: Tag) -> None:
        self.info[node].apply(val, l, r)
        self.tag[node].apply(val)

    def _pushdown(self, node: int, l: int, r: int) -> None:
        if self.tag[node].empty():
            return
        m = (l + r) >> 1
        self._apply(node << 1, l, m, self.tag[node])
        self._apply(node << 1 | 1, m + 1, r, self.tag[node])
        self.tag[node] = Tag()

    def _maintain(self, node: int) -> None:
        self.info[node] = self.info[node << 1] + self.info[node << 1 | 1]

    def _build(self, a: list[Info], node: int, l: int, r: int) -> None:
        if l == r:
            self.info[node] = copy(a[l])
            return
        m = (l + r) >> 1
        self._build(a, node << 1, l, m)
        self._build(a, node << 1 | 1, m + 1, r)
        self._maintain(node)

    def _update(self, node: int, l: int, r: int, ql: int, qr: int, val: Tag) -> None:
        if ql <= l and r <= qr:
            self._apply(node, l, r, val)
            return
        self._pushdown(node, l, r)
        m = (l + r) >> 1
        if ql <= m:
            self._update(node << 1, l, m, ql, qr, val)
        if qr > m:
            self._update(node << 1 | 1, m + 1, r, ql, qr, val)
        self._maintain(node)

    def _assign(self, node: int, l: int, r: int, i: int, val: Info) -> None:
        if l == r:
            self.info[node] = copy(val)
            self.tag[node] = Tag()
            return
        self._pushdown(node, l, r)
        m = (l + r) >> 1
        if i <= m:
            self._assign(node << 1, l, m, i, val)
        else:
            self._assign(node << 1 | 1, m + 1, r, i, val)
        self._maintain(node)

    def _query(self, node: int, l: int, r: int, ql: int, qr: int) -> Info:
        if ql <= l and r <= qr:
            return copy(self.info[node])
        self._pushdown(node, l, r)
        m = (l + r) >> 1
        if qr <= m:
            return self._query(node << 1, l, m, ql, qr)
        if ql > m:
            return self._query(node << 1 | 1, m + 1, r, ql, qr)
        return self._query(node << 1, l, m, ql, qr) + self._query(
            node << 1 | 1, m + 1, r, ql, qr
        )

    def _find_first(
        self, node: int, l: int, r: int, ql: int, qr: int, val: Info
    ) -> int:
        if r < ql or qr < l or self.info[node] < val:
            return -1
        if l == r:
            return l
        self._pushdown(node, l, r)
        m = (l + r) >> 1
        res = self._find_first(node << 1, l, m, ql, qr, val)
        if res != -1:
            return res
        return self._find_first(node << 1 | 1, m + 1, r, ql, qr, val)

    def _find_last(self, node: int, l: int, r: int, ql: int, qr: int, val: Info) -> int:
        if r < ql or qr < l or self.info[node] < val:
            return -1
        if l == r:
            return l
        self._pushdown(node, l, r)
        m = (l + r) >> 1
        res = self._find_last(node << 1 | 1, m + 1, r, ql, qr, val)
        if res != -1:
            return res
        return self._find_last(node << 1, l, m, ql, qr, val)

    def update(self, ql: int, qr: int, val: Tag) -> None:
        self._update(1, 0, self.n - 1, ql, qr, val)  # 修改闭区间 [ql, qr]

    def assign(self, i: int, val: Info) -> None:
        self._assign(1, 0, self.n - 1, i, val)

    def query(self, ql: int, qr: int) -> Info:
        return self._query(1, 0, self.n - 1, ql, qr)

    def find_first(self, ql: int, qr: int, val: Info) -> int:
        return self._find_first(1, 0, self.n - 1, ql, qr, val)

    def find_last(self, ql: int, qr: int, val: Info) -> int:
        return self._find_last(1, 0, self.n - 1, ql, qr, val)


# 用法（n > 0；下标均为 0-based；区间均为闭区间）
# tree = LazySegmentTree([Info(x) for x in a])
# tree = LazySegmentTree(n, Info(0))
# tree.update(l, r, Tag(x))                 # 区间加 x
# tree.assign(i, Info(x))                   # 单点赋值
# res = tree.query(l, r)                    # res.sum / res.mx
# p = tree.find_first(l, r, Info(x))        # 第一个 a[p] >= x，不存在返回 -1
# p = tree.find_last(l, r, Info(x))         # 最后一个 a[p] >= x，不存在返回 -1
# 换题时修改 Info.apply、Info.__add__、Tag.empty 和 Tag.apply
