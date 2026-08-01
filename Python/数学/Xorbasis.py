from typing import List  # noqa: UP035


# 模板题，最大子序列异或和
class XorBasis:
    def __init__(self, n: int) -> None:
        self.b = [0] * n

    def insert(self, x: int) -> None:
        for i in range(len(self.b) - 1, -1, -1):
            if x >> i:
                if self.b[i] == 0:
                    self.b[i] = x
                    return
                x ^= self.b[i]

    def max_xor(self) -> int:
        res = 0
        for i in range(len(self.b) - 1, -1, -1):
            res = max(res, res ^ self.b[i])
        return res


class Solution:
    def maxXorSubsequences(self, nums: List[int]) -> int:  # noqa: UP006
        m = max(nums).bit_length()
        b = XorBasis(m)
        for x in nums:
            b.insert(x)
        return b.max_xor()


# 用法：
# basis = XorBasis(max_bit)
# for x in nums:
#     basis.insert(x)
# ans = basis.max_xor()
