from bisect import bisect_left, bisect_right


class ChairmanTree:
    class Node:
        def __init__(
            self, ls: int = 0, rs: int = 0, cnt: int = 0, sum: int = 0
        ) -> None:
            self.ls = ls
            self.rs = rs
            self.cnt = cnt
            self.sum = sum

    def __init__(self, a: list[int]) -> None:
        self.vals = sorted(set(a))
        n = len(a)
        self.tr = [self.Node()]
        self.root = [0] * (n + 1)

        self.root[0] = self.build(1, len(self.vals))
        for i in range(1, n + 1):
            pos = bisect_left(self.vals, a[i - 1]) + 1
            self.root[i] = self.add(self.root[i - 1], 1, len(self.vals), pos, a[i - 1])

    def build(self, l: int, r: int) -> int:
        o = self.new_node()
        if l == r:
            return o
        m = (l + r) >> 1
        self.tr[o].ls = self.build(l, m)
        self.tr[o].rs = self.build(m + 1, r)
        return o

    def add(self, old: int, l: int, r: int, p: int, val: int) -> int:
        o = self.copy_node(old)
        self.tr[o].cnt += 1
        self.tr[o].sum += val
        if l == r:
            return o
        m = (l + r) >> 1
        if p <= m:
            self.tr[o].ls = self.add(self.tr[old].ls, l, m, p, val)
        else:
            self.tr[o].rs = self.add(self.tr[old].rs, m + 1, r, p, val)
        return o

    def kth(self, l: int, r: int, k: int) -> int:
        return self.vals[
            self._kth(self.root[l - 1], self.root[r], 1, len(self.vals), k) - 1
        ]

    def leq(self, l: int, r: int, x: int) -> tuple[int, int]:
        pos = bisect_right(self.vals, x)
        if not pos:
            return 0, 0
        return self.query(self.root[l - 1], self.root[r], 1, len(self.vals), pos)

    def new_node(self) -> int:
        self.tr.append(self.Node())
        return len(self.tr) - 1

    def copy_node(self, old: int) -> int:
        x = self.tr[old]
        self.tr.append(self.Node(x.ls, x.rs, x.cnt, x.sum))
        return len(self.tr) - 1

    # C++ 通过重载复用 kth；Python 将递归版本最小避名为 _kth
    def _kth(self, old: int, now: int, l: int, r: int, k: int) -> int:
        if l == r:
            return l
        left_cnt = self.tr[self.tr[now].ls].cnt - self.tr[self.tr[old].ls].cnt
        m = (l + r) >> 1
        if k <= left_cnt:
            return self._kth(self.tr[old].ls, self.tr[now].ls, l, m, k)
        return self._kth(self.tr[old].rs, self.tr[now].rs, m + 1, r, k - left_cnt)

    def query(self, old: int, now: int, l: int, r: int, qr: int) -> tuple[int, int]:
        if r <= qr:
            return self.tr[now].cnt - self.tr[old].cnt, self.tr[now].sum - self.tr[
                old
            ].sum
        m = (l + r) >> 1
        cnt, sum = self.query(self.tr[old].ls, self.tr[now].ls, l, m, qr)
        if qr > m:
            cnt2, sum2 = self.query(self.tr[old].rs, self.tr[now].rs, m + 1, r, qr)
            cnt += cnt2
            sum += sum2
        return cnt, sum


# 使用：ct = ChairmanTree(a)
# ct.kth(l, r, k)：查询 1-indexed 区间 [l, r] 第 k 小的原值
# ct.leq(l, r, x)：查询区间 [l, r] 内 <= x 的数量和总和
#
# a = [...]                         # a 是 0-indexed 原数组
# ct = ChairmanTree(a)
# kth_val = ct.kth(l, r, k)         # l, r, k 都按 1-indexed 传
# cnt, sum = ct.leq(l, r, limit)


# C++ 中两份备选结构同名，这里将第二份最小避名为 ChairmanTreeLite
# 常数较小的主席树（和上面的版本二选一复制；复制时可改名回 ChairmanTree）
#
# 用法：
#     a = [...]                         # 原数组，0-indexed
#     ct = ChairmanTreeLite(a)
#
#     v = ct.kth(l, r, k)               # 查询 1-indexed 区间 [l, r] 第 k 小的原值
#     cnt = ct.leq(l, r, x)             # 查询 1-indexed 区间 [l, r] 中 <= x 的数量
#
# 复杂度：
#     建树 O(n log V)，单次 kth / leq 查询 O(log V)，V 为不同值个数。
#
# 为什么常数小：
#     1. 不 build 空树，root[0] 直接用 0 号空节点。每次插入只新建一条链。
#     2. Node 只维护 ls / rs / cnt，没有 sum 等额外字段，节点更小、缓存更友好。
#     3. add / query 少维护一个 sum，递归里的运算更少。
#
# 什么时候用它：
#     1. 只需要区间第 k 小、区间 <= x 的数量、排名/计数这类“只看个数”的查询。
#     2. 题目卡常或 n 较大，内存、时间都比较紧。
#
# 什么时候用上面的版本：
#     1. 需要同时查询区间 <= x 的数量和总和：cnt, sum = ct.leq(l, r, x)。
#     2. 需要把节点扩展成维护 sum / max / 其它统计量。
#     3. 想保留一棵显式 build 出来的空树，写扩展查询时更直观。
class ChairmanTreeLite:
    class Node:
        def __init__(self, ls: int = 0, rs: int = 0, cnt: int = 0) -> None:
            self.ls = ls
            self.rs = rs
            self.cnt = cnt

    def __init__(self, a: list[int]) -> None:
        self.vals = sorted(set(a))
        n = len(a)
        self.tr = [self.Node()]
        self.root = [0] * (n + 1)

        self.root[0] = 0
        for i in range(1, n + 1):
            pos = bisect_left(self.vals, a[i - 1]) + 1
            self.root[i] = self.add(self.root[i - 1], 1, len(self.vals), pos)

    def add(self, old: int, l: int, r: int, p: int) -> int:
        o = self.copy_node(old)
        self.tr[o].cnt += 1
        if l == r:
            return o
        m = (l + r) >> 1
        if p <= m:
            self.tr[o].ls = self.add(self.tr[old].ls, l, m, p)
        else:
            self.tr[o].rs = self.add(self.tr[old].rs, m + 1, r, p)
        return o

    def kth(self, l: int, r: int, k: int) -> int:
        return self.vals[
            self._kth(self.root[l - 1], self.root[r], 1, len(self.vals), k) - 1
        ]

    def leq(self, l: int, r: int, x: int) -> int:
        pos = bisect_right(self.vals, x)
        if not pos:
            return 0
        return self.query(self.root[l - 1], self.root[r], 1, len(self.vals), pos)

    def copy_node(self, old: int) -> int:
        x = self.tr[old]
        self.tr.append(self.Node(x.ls, x.rs, x.cnt))
        return len(self.tr) - 1

    # C++ 通过重载复用 kth；Python 将递归版本最小避名为 _kth
    def _kth(self, old: int, now: int, l: int, r: int, k: int) -> int:
        if l == r:
            return l
        left_cnt = self.tr[self.tr[now].ls].cnt - self.tr[self.tr[old].ls].cnt
        m = (l + r) >> 1
        if k <= left_cnt:
            return self._kth(self.tr[old].ls, self.tr[now].ls, l, m, k)
        return self._kth(self.tr[old].rs, self.tr[now].rs, m + 1, r, k - left_cnt)

    def query(self, old: int, now: int, l: int, r: int, qr: int) -> int:
        if r <= qr:
            return self.tr[now].cnt - self.tr[old].cnt
        m = (l + r) >> 1
        res = self.query(self.tr[old].ls, self.tr[now].ls, l, m, qr)
        if qr > m:
            res += self.query(self.tr[old].rs, self.tr[now].rs, m + 1, r, qr)
        return res
