from dataclasses import dataclass
from typing import Callable, List, TypeVar


T = TypeVar("T")


class SegmentTree:
    # 维护整棵树

    def __init__(self, a: List[T] | int, init_val: T = 0, merge: Callable[[T, T], T] | None = None):
        # 传入一个数组维护
        if isinstance(a, int):
            a = [init_val] * a
        self.n = len(a)
        self.merge = merge or max  # 合并子树
        self.tree = [init_val] * (4 * self.n)
        self._build(a, 1, 0, self.n - 1)

    def _build(self, a: List[T], o: int, l: int, r: int) -> None:
        if l == r:
            self.tree[o] = a[l]
            return
        m = (l + r) >> 1
        self._build(a, o << 1, l, m)
        self._build(a, o << 1 | 1, m + 1, r)
        self.tree[o] = self.merge(self.tree[o << 1], self.tree[o << 1 | 1])
        # 建树

    def update(self, pos: int, val: T) -> None:
        # 更新i的值为val
        self._update(1, 0, self.n - 1, pos, val)

    def _update(self, o: int, l: int, r: int, pos: int, val: T) -> None:
        if l == r:
            self.tree[o] = val
            return
        m = (l + r) >> 1
        if pos <= m:
            self._update(o << 1, l, m, pos, val)
        else:
            self._update(o << 1 | 1, m + 1, r, pos, val)
        self.tree[o] = self.merge(self.tree[o << 1], self.tree[o << 1 | 1])
        # 更新i处的值为val

    def query(self, ql: int, qr: int) -> T:
        # 查询[ql,qr]的值
        return self._query(1, 0, self.n - 1, ql, qr)

    def _query(self, o: int, l: int, r: int, ql: int, qr: int) -> T:
        if ql <= l and r <= qr:
            return self.tree[o]
        m = (l + r) >> 1
        if qr <= m:
            return self._query(o << 1, l, m, ql, qr)
        if ql > m:
            return self._query(o << 1 | 1, m + 1, r, ql, qr)
        l_res = self._query(o << 1, l, m, ql, qr)
        r_res = self._query(o << 1 | 1, m + 1, r, ql, qr)
        return self.merge(l_res, r_res)  # 查询[ql,qr]的值

    def get(self, pos: int) -> T:
        # 取出i处的值
        return self.query(pos, pos)

    def find_first_at_least(self, ql: int, qr: int, val: T) -> int:
        # 查询[ql,qr]中第一个满足条件的下标
        return self._find_first(1, 0, self.n - 1, ql, qr, val)

    def _find_first(self, o: int, l: int, r: int, ql: int, qr: int, val: T) -> int:
        # 若遇到固定左端点的情况，需要使用全局变量（或者传入引用）记录前缀分段最大值，加一个被待求区间完全覆盖的剪枝
        if r < ql or qr < l or self.tree[o] < val:
            return -1
        if l == r:
            return l
        m = (l + r) >> 1
        res = self._find_first(o << 1, l, m, ql, qr, val)
        if res != -1:
            return res
        return self._find_first(o << 1 | 1, m + 1, r, ql, qr, val)

    def find_last_at_least(self, ql: int, qr: int, val: T) -> int:
        # 查询[ql,qr]中最后一个满足条件的下标
        return self._find_last(1, 0, self.n - 1, ql, qr, val)

    def _find_last(self, o: int, l: int, r: int, ql: int, qr: int, val: T) -> int:
        if r < ql or qr < l or self.tree[o] < val:
            return -1
        if l == r:
            return l
        m = (l + r) >> 1
        res = self._find_last(o << 1 | 1, m + 1, r, ql, qr, val)
        if res != -1:
            return res
        return self._find_last(o << 1, l, m, ql, qr, val)


@dataclass
class MaxSubarrayInfo:
    total: int = 0
    pre: int = 0
    suf: int = 0
    ans: int = 0

    @classmethod
    def from_value(cls, x: int) -> "MaxSubarrayInfo":
        return cls(x, x, x, x)


def merge_max_subarray(a: MaxSubarrayInfo, b: MaxSubarrayInfo) -> MaxSubarrayInfo:
    # CF1906F
    # 引入了维护Info，重载加法的新想法
    # 维护最大子段和，离线处理，换维度
    # 好题
    return MaxSubarrayInfo(
        total=a.total + b.total,
        pre=max(a.pre, a.total + b.pre),
        suf=max(b.suf, b.total + a.suf),
        ans=max(a.ans, b.ans, a.suf + b.pre),
    )
