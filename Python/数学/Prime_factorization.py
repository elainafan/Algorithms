from typing import List, Tuple


def least_prime_factors(mx: int) -> List[int]:
    lpf = [0] * mx  # 存储每个数的最小素因子，复杂度O(NloglogN)
    for i in range(2, mx):
        if lpf[i] == 0:
            for j in range(i, mx, i):
                if lpf[j] == 0:
                    lpf[j] = i
    return lpf


def cnt(x: int, lpf: List[int] | None = None) -> List[Tuple[int, int]]:
    # 质因数分解，返回值为pair<素因子，素因子次幂>，复杂度O(logN)
    res = []
    if lpf is not None and x < len(lpf):
        while x > 1:
            p = lpf[x]
            e = 1
            x //= p
            while x % p == 0:
                x //= p
                e += 1
            res.append((p, e))
        return res

    i = 2
    while i * i <= x:
        if x % i == 0:
            e = 0
            while x % i == 0:
                x //= i
                e += 1
            res.append((i, e))
        i += 1
    if x > 1:
        res.append((x, 1))
    return res
