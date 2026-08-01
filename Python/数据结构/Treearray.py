class Tree:
    def __init__(self, a, init_val: int = 0) -> None:
        if isinstance(a, int):
            n = a
            self.tree = [0] * (n + 1)
            for i in range(1, n + 1):
                self.tree[i] += init_val
                j = i + (i & -i)
                if j <= n:
                    self.tree[j] += self.tree[i]
        else:
            n = len(a)
            self.tree = [0] * (n + 1)
            for i, x in enumerate(a, 1):
                self.tree[i] += x
                j = i + (i & -i)
                if j <= n:
                    self.tree[j] += self.tree[i]

    def add(self, i: int, val: int = 1) -> None:
        i += 1
        while i < len(self.tree):
            self.tree[i] += val
            i += i & -i

    def pre(self, i: int) -> int:
        res = 0
        i += 1
        while i > 0:
            res += self.tree[i]
            i &= i - 1
        return res

    def query(self, l: int, r: int) -> int:
        if r < l:
            return 0
        return self.pre(r) - self.pre(l - 1)

    def lower_bound(self, val: int) -> int:
        n = len(self.tree) - 1
        res = 0
        cur = 0
        bit = 1 << (n.bit_length() - 1) if n else 0
        while bit:
            nxt = res + bit
            if nxt <= n and cur + self.tree[nxt] < val:
                res = nxt
                cur += self.tree[nxt]
            bit >>= 1
        return res


class MaxTree:
    def __init__(self, a, identity: float = float("-inf")) -> None:
        self.identity = identity
        if isinstance(a, int):
            self.tree = [identity] * (a + 1)
            return
        n = len(a)
        self.tree = [identity] * (n + 1)
        for i, x in enumerate(a, 1):
            self.tree[i] = max(self.tree[i], x)
            j = i + (i & -i)
            if j <= n:
                self.tree[j] = max(self.tree[j], self.tree[i])

    def chmax(self, i: int, val: int) -> None:
        i += 1
        while i < len(self.tree):
            self.tree[i] = max(self.tree[i], val)
            i += i & -i

    def pre(self, i: int) -> float:
        res = self.identity
        i += 1
        while i > 0:
            res = max(res, self.tree[i])
            i &= i - 1
        return res


# 用法（下标均为 0-based；区间均为闭区间）
# tree = Tree(a)
# tree = Tree(n, init_val)
# tree.add(i, x)
# s = tree.pre(i)          # [0, i] 的和
# s = tree.query(l, r)     # [l, r] 的和
# i = tree.lower_bound(x)  # 最小的 i 使 pre(i) >= x；不存在返回 n
# lower_bound 要求前缀和单调，通常即数组元素非负
#
# tree = MaxTree(a)
# tree.chmax(i, x)         # 只支持单点变大
# mx = tree.pre(i)         # [0, i] 的最大值
