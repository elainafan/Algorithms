import sys


# 笛卡尔树，默认为大根堆
class CartesianTree:
    class Node:
        def __init__(
            self,
            idx: int = 0,
            val=0,
            par: int = 0,
            siz: int = 0,
        ) -> None:
            self.idx = idx  # 原数组下标
            self.val = val  # 权值
            self.par = par  # 父节点索引
            self.siz = siz  # 子树大小
            self.son = [0, 0]  # 左儿子，右儿子

    def __init__(self, comp=None) -> None:
        self.inf = float("inf")
        self.comp = comp if comp is not None else lambda x, y: x > y
        self.t = []
        self.init()

    def init(self, comp=None) -> None:
        if comp is not None:
            self.comp = comp
        comp = self.comp
        self.t = [self.Node()]
        if comp(-self.inf, self.inf):
            self.t[0].val = -self.inf
        else:
            self.t[0].val = self.inf
        # 自动建立虚拟节点

    # 负责把元素加进末尾，需要使用1-based
    def add(self, idx: int, val, par: int = 0) -> None:
        self.t.append(self.Node(idx, val, par))

    def work(self, comp=None) -> int:
        if comp is None:
            comp = self.comp
        sys.setrecursionlimit(max(sys.getrecursionlimit(), len(self.t) * 2 + 10))
        for i in range(1, len(self.t)):
            k = i - 1
            while comp(self.t[i].val, self.t[k].val):
                k = self.t[k].par
            self.t[i].son[0] = self.t[k].son[1]
            self.t[k].son[1] = i
            self.t[i].par = k
            self.t[self.t[i].son[0]].par = i

        # 遍历，砍树枝
        def dfs(u: int) -> None:
            if not u:
                return
            self.t[u].siz = 1
            dfs(self.ls(u))
            dfs(self.rs(u))
            self.t[u].siz += self.t[self.ls(u)].siz + self.t[self.rs(u)].siz

        # 进行一个dfs
        dfs(self.t[0].son[1])
        return self.t[0].son[1]

    # 左边最远
    def Left(self, p: int) -> int:
        return p - self.size(self.ls(p))

    # 右边最远
    def Right(self, p: int) -> int:
        return p + self.size(self.rs(p))

    def size(self, p: int) -> int:
        return self.t[p].siz

    def ls(self, p: int) -> int:
        return self.t[p].son[0]

    def rs(self, p: int) -> int:
        return self.t[p].son[1]

    def par(self, p: int) -> int:
        return self.t[p].par


# 使用示例
# ct = CartesianTree(lambda x, y: x < y)
# for i in range(n):
#     ct.add(i + 1, nums[i])
#
# root = ct.work(lambda x, y: x < y)
# for i in range(1, n + 1):
#     L = ct.Left(i)
#     R = ct.Right(i)
