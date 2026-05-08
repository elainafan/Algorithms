# 普通并查集

from collections import defaultdict, deque, Counter
from functools import cache
from sortedcontainers import SortedSet, SortedDict, SortedList
from typing import List, Tuple, Set
from math import inf
from heapq import heappop, heappush
from bisect import bisect_left, bisect_right
import math


class UnionFind:
    def __init__(self, n: int):
        self.fa = list(range(n))
        self.siz = [1] * n  # 集合大小
        self.cc = n  # 连通块个数

    def get(self, x: int) -> int:
        if self.fa[x] != x:
            self.fa[x] = self.get(self.fa[x])
        return self.fa[x]

    def find(self, x: int) -> int:
        return self.get(x)

    def is_same(self, x: int, y: int) -> bool:
        return self.get(x) == self.get(y)

    def merge(self, from_: int, to: int) -> bool:
        x, y = self.get(from_), self.get(to)
        if x == y:
            return False
        self.fa[x] = y
        self.siz[y] += self.siz[x]
        self.cc -= 1
        return True

    def get_size(self, x: int) -> int:
        # 查询x所在集合大小
        return self.siz[self.get(x)]
