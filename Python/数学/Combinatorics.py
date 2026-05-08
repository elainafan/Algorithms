from typing import List


MOD = 10**9 + 7


class Comb:
    def __init__(self, n: int, mod: int = MOD):
        self.mod = mod
        self.F = [1] * (n + 1)  # 预处理阶乘
        self.INV_F = [1] * (n + 1)  # 预处理逆元
        for i in range(1, n + 1):
            self.F[i] = self.F[i - 1] * i % mod  # 预处理阶乘
        self.INV_F[n] = pow(self.F[n], mod - 2, mod)
        for i in range(n, 0, -1):
            self.INV_F[i - 1] = self.INV_F[i] * i % mod
        # 预处理逆元

    def comb(self, n: int, m: int) -> int:
        # 计算C(n,m),即从n个数中取m个数
        if m < 0 or m > n:
            return 0
        return self.F[n] * self.INV_F[m] % self.mod * self.INV_F[n - m] % self.mod


def pascal(mx: int) -> List[List[int]]:
    c = [[0] * (mx + 1) for _ in range(mx + 1)]  # 即为C(n,m),从n个数中取m个数
    for i in range(mx + 1):
        c[i][0] = c[i][i] = 1
        for j in range(1, i):
            c[i][j] = c[i - 1][j - 1] + c[i - 1][j]
    # 适用于MX较小的情况
    return c
