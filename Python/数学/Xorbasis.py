from typing import Iterable


class XorBasis:
    # 模板题，最大子序列异或和
    def __init__(self, max_bit: int = 60):
        self.b = [0] * max_bit

    def insert(self, x: int) -> bool:
        for i in range(len(self.b) - 1, -1, -1):
            if not (x >> i) & 1:
                continue
            if self.b[i] == 0:
                self.b[i] = x
                return True
            x ^= self.b[i]
        return False

    def max_xor(self) -> int:
        res = 0
        for x in reversed(self.b):
            res = max(res, res ^ x)
        return res


def max_xor_subsequence(nums: Iterable[int]) -> int:
    nums = list(nums)
    max_bit = max(nums).bit_length() if nums else 1
    xb = XorBasis(max_bit)
    for x in nums:
        xb.insert(x)
    return xb.max_xor()
