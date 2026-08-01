from copy import copy


class Info:
    __slots__ = ("ans", "mx", "pre", "suf", "sum")

    def __init__(self, x: int = 0) -> None:
        self.sum = self.pre = self.suf = self.ans = self.mx = x

    def __add__(self, other: "Info") -> "Info":
        res = Info()
        res.sum = self.sum + other.sum
        res.pre = max(self.pre, self.sum + other.pre)
        res.suf = max(other.suf, other.sum + self.suf)
        res.ans = max(self.ans, other.ans, self.suf + other.pre)
        res.mx = max(self.mx, other.mx)
        return res

    def __lt__(self, other: "Info") -> bool:
        return self.mx < other.mx  # 线段树二分用，不需要时可删


class SegmentTree:
    def __init__(self, a, init_val=None) -> None:
        if isinstance(a, int):
            init_val = Info() if init_val is None else init_val
            a = [copy(init_val) for _ in range(a)]
        self.n = len(a)
        self.tree = [Info() for _ in range(4 * self.n)]
        self._build(a, 1, 0, self.n - 1)

    def _maintain(self, node: int) -> None:
        self.tree[node] = self.tree[node << 1] + self.tree[node << 1 | 1]

    def _build(self, a: list[Info], node: int, l: int, r: int) -> None:
        if l == r:
            self.tree[node] = copy(a[l])
            return
        m = (l + r) >> 1
        self._build(a, node << 1, l, m)
        self._build(a, node << 1 | 1, m + 1, r)
        self._maintain(node)

    def _update(self, node: int, l: int, r: int, i: int, val: Info) -> None:
        if l == r:
            self.tree[node] = copy(val)
            return
        m = (l + r) >> 1
        if i <= m:
            self._update(node << 1, l, m, i, val)
        else:
            self._update(node << 1 | 1, m + 1, r, i, val)
        self._maintain(node)

    def _query(self, node: int, l: int, r: int, ql: int, qr: int) -> Info:
        if ql <= l and r <= qr:
            return copy(self.tree[node])
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
        if r < ql or qr < l or self.tree[node] < val:
            return -1
        if l == r:
            return l
        m = (l + r) >> 1
        res = self._find_first(node << 1, l, m, ql, qr, val)
        if res != -1:
            return res
        return self._find_first(node << 1 | 1, m + 1, r, ql, qr, val)

    def _find_last(self, node: int, l: int, r: int, ql: int, qr: int, val: Info) -> int:
        if r < ql or qr < l or self.tree[node] < val:
            return -1
        if l == r:
            return l
        m = (l + r) >> 1
        res = self._find_last(node << 1 | 1, m + 1, r, ql, qr, val)
        if res != -1:
            return res
        return self._find_last(node << 1, l, m, ql, qr, val)

    def update(self, i: int, val: Info) -> None:
        self._update(1, 0, self.n - 1, i, val)  # 将 i 处改为 val

    def query(self, ql: int, qr: int) -> Info:
        return self._query(1, 0, self.n - 1, ql, qr)  # 查询闭区间 [ql, qr]

    def get(self, i: int) -> Info:
        return self.query(i, i)

    def find_first(self, ql: int, qr: int, val: Info) -> int:
        return self._find_first(1, 0, self.n - 1, ql, qr, val)

    def find_last(self, ql: int, qr: int, val: Info) -> int:
        return self._find_last(1, 0, self.n - 1, ql, qr, val)


# 用法（n > 0；下标均为 0-based；区间均为闭区间）
# tree = SegmentTree([Info(x) for x in a])
# tree = SegmentTree(n, Info(0))
# tree.update(i, Info(x))
# ans = tree.query(l, r).ans
# x = tree.get(i).sum
# p = tree.find_first(l, r, Info(x))    # 第一个 a[p] >= x，不存在返回 -1
# p = tree.find_last(l, r, Info(x))     # 最后一个 a[p] >= x，不存在返回 -1
# 当前 Info 维护最大子段和；换题时按需修改 Info 和 __add__
