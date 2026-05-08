from typing import List


class LazySegmentTree:
    def __init__(self, a: List[int] | int, init_val: int = 0):
        # 维护下标为[0,n-1],初始值为init_val的区间，或者数组a
        if isinstance(a, int):
            a = [init_val] * a
        self.n = len(a)
        self.val = [0] * (4 * self.n)
        self.mx = [0] * (4 * self.n)
        self.todo = [0] * (4 * self.n)  # 懒标记初始值
        self._build(a, 1, 0, self.n - 1)

    def _pull(self, o: int) -> None:
        # 合并两个val
        # 合并线段树
        self.val[o] = self.val[o << 1] + self.val[o << 1 | 1]
        self.mx[o] = max(self.mx[o << 1], self.mx[o << 1 | 1])

    def _apply(self, o: int, l: int, r: int, v: int) -> None:
        # 把懒标记作用到node子树
        self.val[o] += v * (r - l + 1)
        self.mx[o] += v
        self.todo[o] += v
        # 合并两个懒标记

    def _push(self, o: int, l: int, r: int) -> None:
        # 把当前节点的懒标记下传
        if self.todo[o] == 0 or l == r:
            return
        m = (l + r) >> 1
        v = self.todo[o]
        self._apply(o << 1, l, m, v)
        self._apply(o << 1 | 1, m + 1, r, v)
        self.todo[o] = 0

    def _build(self, a: List[int], o: int, l: int, r: int) -> None:
        if l == r:
            self.val[o] = self.mx[o] = a[l]
            return
        m = (l + r) >> 1
        self._build(a, o << 1, l, m)
        self._build(a, o << 1 | 1, m + 1, r)
        self._pull(o)
        # 建树，复杂度O(n)

    def add(self, ql: int, qr: int, v: int) -> None:
        # 更新[ql,qr]为f
        self._add(1, 0, self.n - 1, ql, qr, v)

    def _add(self, o: int, l: int, r: int, ql: int, qr: int, v: int) -> None:
        if ql <= l and r <= qr:
            self._apply(o, l, r, v)
            return
        self._push(o, l, r)
        m = (l + r) >> 1
        if ql <= m:
            self._add(o << 1, l, m, ql, qr, v)
        if qr > m:
            self._add(o << 1 | 1, m + 1, r, ql, qr, v)
        self._pull(o)
        # 区间更新[ql,qr]

    def query(self, ql: int, qr: int) -> int:
        # 区间查询[ql,qr]
        return self._query(1, 0, self.n - 1, ql, qr)

    def _query(self, o: int, l: int, r: int, ql: int, qr: int) -> int:
        if ql <= l and r <= qr:
            return self.val[o]
        self._push(o, l, r)
        m = (l + r) >> 1
        ans = 0
        if ql <= m:
            ans += self._query(o << 1, l, m, ql, qr)
        if qr > m:
            ans += self._query(o << 1 | 1, m + 1, r, ql, qr)
        # 区间查找
        return ans

    def find_first_at_least(self, ql: int, qr: int, val: int) -> int:
        # 查询[ql,qr]中第一个满足条件的下标
        return self._find_first(1, 0, self.n - 1, ql, qr, val)

    def _find_first(self, o: int, l: int, r: int, ql: int, qr: int, val: int) -> int:
        # 若遇到固定左端点的情况，需要使用全局变量（或者传入引用）记录前缀分段最大值，加一个被待求区间完全覆盖的剪枝
        if r < ql or qr < l or self.mx[o] < val:
            return -1
        if l == r:
            return l
        self._push(o, l, r)
        m = (l + r) >> 1
        res = self._find_first(o << 1, l, m, ql, qr, val)
        if res != -1:
            return res
        return self._find_first(o << 1 | 1, m + 1, r, ql, qr, val)

    def find_last_at_least(self, ql: int, qr: int, val: int) -> int:
        # 查询[ql,qr]中最后一个满足条件的下标
        return self._find_last(1, 0, self.n - 1, ql, qr, val)

    def _find_last(self, o: int, l: int, r: int, ql: int, qr: int, val: int) -> int:
        if r < ql or qr < l or self.mx[o] < val:
            return -1
        if l == r:
            return l
        self._push(o, l, r)
        m = (l + r) >> 1
        res = self._find_last(o << 1 | 1, m + 1, r, ql, qr, val)
        if res != -1:
            return res
        return self._find_last(o << 1, l, m, ql, qr, val)


# 注：懒标记线段树无论做什么都需要pushdown
# 此时其它与线段树二分同
# 双标记，注意顺序
# 合并两个乘法标记
# LC850:矩形面积并（扫描线）
# 区间内被覆盖的最小次数
# 区间内为最小次数的区间长度
# 懒标记
# 根据左右儿子的信息，更新当前节点的信息
# 仅更新节点信息，不下传懒标记
# 下传懒标记
# 有这么多个差值
# 根节点是1
# 注意点和区间的对应关系


RangeAddSegmentTree = LazySegmentTree
