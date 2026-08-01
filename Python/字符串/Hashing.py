from time import monotonic_ns

MASK = (1 << 64) - 1


def splitmix64(x: int) -> int:
    x = (x + 0x9E3779B97F4A7C15) & MASK
    x = ((x ^ (x >> 30)) * 0xBF58476D1CE4E5B9) & MASK
    x = ((x ^ (x >> 27)) * 0x94D049BB133111EB) & MASK
    return (x ^ (x >> 31)) & MASK


class custom_hash:
    FIXED_RANDOM = monotonic_ns()

    def __call__(self, x: int) -> int:
        return splitmix64(x + self.FIXED_RANDOM)


class StringHash:
    _base = 0

    @staticmethod
    def base() -> int:
        if StringHash._base == 0:
            StringHash._base = splitmix64(monotonic_ns()) | 1
        return StringHash._base

    def __init__(self, s: str) -> None:
        n = len(s)
        self.h = [0] * (n + 1)
        self.p = [1] * (n + 1)
        base = self.base()
        for i in range(n):
            self.h[i + 1] = (self.h[i] * base + (ord(s[i]) & 255) + 1) & MASK
            self.p[i + 1] = self.p[i] * base & MASK

    # 获取 s[l...r] 的哈希值，闭区间，0-indexed
    def query(self, l: int, r: int) -> int:
        return (self.h[r + 1] - self.h[l] * self.p[r - l + 1]) & MASK


# 使用：
# hs = StringHash(s)
# value = hs.query(l, r)
#
# Python 的 dict 不能像 C++ unordered_map 一样传入自定义哈希器。
# 如果只想手动混淆整数键，可以写 value = custom_hash()(x)
