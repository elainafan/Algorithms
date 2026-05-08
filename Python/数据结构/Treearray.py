from typing import List


class Fenwick:
    def __init__(self, data: int | List[int], init_val: int = 0):
        if isinstance(data, int):
            # 构造函数：初始化大小为 n 的树状数组，初始所有元素值为 val（外部表现为 0-based）
            n = data
            self.tree = [0] * (n + 1)
            for i in range(1, n + 1):
                self.tree[i] += init_val
                j = i + (i & -i)
                if j <= n:
                    self.tree[j] += self.tree[i]
        else:
            # 构造函数：使用给定的 vector 在 O(N) 时间内快速初始化建树
            n = len(data)
            self.tree = [0] * (n + 1)
            for i, x in enumerate(data, 1):
                self.tree[i] += x  # data是 0-based
                j = i + (i & -i)
                if j <= n:
                    self.tree[j] += self.tree[i]

    def add(self, i: int, val: int = 1) -> None:
        # 单点修改：将 0-based 下标 i 处的元素增加 val
        i += 1
        while i < len(self.tree):
            self.tree[i] += val
            i += i & -i

    def pre(self, i: int) -> int:
        # 前缀求和：计算 0-based 下标区间 [0, i] 内的所有元素之和
        if i < 0:
            return 0
        i += 1
        ans = 0
        while i > 0:
            ans += self.tree[i]
            i &= i - 1
        return ans

    def query(self, l: int, r: int) -> int:
        # 区间求和：计算 0-based 下标区间 [l, r] 内的所有元素之和
        if r < l:
            return 0
        return self.pre(r) - self.pre(l - 1)  # 当 l=0 时, pre(-1) 会合理地返回 0

    def lower_bound(self, val: int) -> int:
        # 树上二分查找：返回满足前缀和 >= val 的最小 0-based 下标
        pos = 0
        cur = 0
        bit = 1 << (len(self.tree) - 1).bit_length()
        while bit:
            nxt = pos + bit
            if nxt < len(self.tree) and cur + self.tree[nxt] < val:
                cur += self.tree[nxt]
                pos = nxt
            bit >>= 1
        return pos  # 返回 0-based 下标：内部 1-based 下标为 res + 1，因此 0-based 为 res
