from dataclasses import dataclass
from math import inf
from typing import Callable, List


@dataclass
class Node:
    idx: int = 0  # 原数组下标
    val: int = 0  # 权值
    par: int = 0  # 父节点索引，子树大小
    size: int = 0
    left: int = 0  # 左儿子，右儿子
    right: int = 0


class CartesianTree:
    # 笛卡尔树，默认为大根堆

    def __init__(self, better: Callable[[int, int], bool] | None = None, sentinel: int | None = None):
        self.better = better or (lambda a, b: a > b)
        if sentinel is None:
            sentinel = inf
        self.t: List[Node] = [Node(val=sentinel)]
        # 自动建立虚拟节点

    def add(self, idx: int, val: int) -> None:
        # 负责把元素加进末尾，需要使用1-based
        self.t.append(Node(idx=idx, val=val))

    def work(self) -> int:
        for i in range(1, len(self.t)):
            k = i - 1
            while self.better(self.t[i].val, self.t[k].val):
                k = self.t[k].par
            self.t[i].left = self.t[k].right
            self.t[k].right = i
            self.t[i].par = k
            self.t[self.t[i].left].par = i
        # 遍历，砍树枝

        root = self.t[0].right

        def dfs(u: int) -> None:
            if u == 0:
                return
            dfs(self.t[u].left)
            dfs(self.t[u].right)
            self.t[u].size = 1 + self.t[self.t[u].left].size + self.t[self.t[u].right].size
        # 进行一个dfs

        dfs(root)
        return root

    def left_bound(self, p: int) -> int:
        # 左边最远
        return p - self.t[self.t[p].left].size

    def right_bound(self, p: int) -> int:
        # 右边最远
        return p + self.t[self.t[p].right].size


# 使用示例
