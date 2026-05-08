from typing import Tuple


# 自定义 pair 的哈希函数，用于 unordered_map
# 简单的组合哈希策略


class StringHash:
    # 双哈希类
    # 第一组参数
    MOD1 = 10**9 + 7
    BASE1 = 131

    # 第二组参数
    MOD2 = 10**9 + 9
    BASE2 = 13331

    def __init__(self, s: str):
        n = len(s)
        self.h1 = [0] * (n + 1)
        self.h2 = [0] * (n + 1)
        self.p1 = [1] * (n + 1)
        self.p2 = [1] * (n + 1)
        for i, ch in enumerate(s):
            x = ord(ch)
            # 哈希 1
            self.h1[i + 1] = (self.h1[i] * self.BASE1 + x) % self.MOD1
            self.p1[i + 1] = self.p1[i] * self.BASE1 % self.MOD1

            # 哈希 2
            self.h2[i + 1] = (self.h2[i] * self.BASE2 + x) % self.MOD2
            self.p2[i + 1] = self.p2[i] * self.BASE2 % self.MOD2

    def query(self, l: int, r: int) -> Tuple[int, int]:
        # 获取 s[l...r] 的双哈希值 (闭区间，0-indexed)
        v1 = (self.h1[r + 1] - self.h1[l] * self.p1[r - l + 1]) % self.MOD1  # 关键：处理负数
        v2 = (self.h2[r + 1] - self.h2[l] * self.p2[r - l + 1]) % self.MOD2  # 关键：处理负数
        return v1, v2


# 使用：unordered_map<pair<ll,ll>,int,pair_hash>ma;
