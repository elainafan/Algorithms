from math import isqrt
from typing import List


class SqrtDecomposition:
    def __init__(self, a: List[int]):
        self.n = len(a)
        self.B = max(1, isqrt(max(1, self.n)))
        self.a = a[:]
        self.block_count = (self.n + self.B - 1) // self.B
        self.sum = [0] * self.block_count
        self.tag = [0] * self.block_count
        for i, x in enumerate(a):
            self.sum[i // self.B] += x

    def _block_len(self, block_id: int) -> int:
        l = block_id * self.B
        r = min(self.n, l + self.B)
        return r - l

    def add_point(self, pos: int, val: int) -> None:
        # 单点加法
        bid = pos // self.B
        self.a[pos] += val
        self.sum[bid] += val

    def add_range(self, l: int, r: int, val: int) -> None:
        # 区间加法
        idl, idr = l // self.B, r // self.B
        if idl == idr:
            for i in range(l, r + 1):
                self.a[i] += val
                self.sum[idl] += val
            return

        for i in range(l, min(self.n, (idl + 1) * self.B)):
            self.a[i] += val
            self.sum[idl] += val
        for i in range(idl + 1, idr):
            self.tag[i] += val
            self.sum[i] += val * self._block_len(i)
        for i in range(idr * self.B, r + 1):
            self.a[i] += val
            self.sum[idr] += val

    def query(self, l: int, r: int) -> int:
        # 带标记区间查询
        # 区间查询
        idl, idr = l // self.B, r // self.B
        if idl == idr:
            return sum(self.a[i] + self.tag[idl] for i in range(l, r + 1))

        ans = 0
        for i in range(l, min(self.n, (idl + 1) * self.B)):
            ans += self.a[i] + self.tag[idl]
        for i in range(idl + 1, idr):
            ans += self.sum[i]
        for i in range(idr * self.B, r + 1):
            ans += self.a[i] + self.tag[idr]
        return ans
