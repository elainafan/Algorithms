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
        self.sz = [1] * n
        self.cc = n

    def find(self, x: int) -> int:
        if self.fa[x] != x:
            self.fa[x] = self.find(self.fa[x])
        return self.fa[x]

    def is_same(self, x: int, y: int) -> bool:
        return self.find(x) == self.find(y)

    def merge(self, _from: int, _to: int) -> bool:
        x, y = self.find(_from), self.find(_to)
        if x == y:
            return False
        self.fa[x] = y
        self.sz[y] += self.sz[x]
        self.cc -= 1
        return True

    def get_size(self, x: int) -> int:
        return self.sz[x]
