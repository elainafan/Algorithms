from typing import List


MOD = 10**9 + 7
Matrix = List[List[int]]


def mul(a: Matrix, b: Matrix, mod: int = MOD) -> Matrix:
    n, m, p = len(a), len(b), len(b[0])
    c = [[0] * p for _ in range(n)]
    for i in range(n):
        for k in range(m):
            if a[i][k] == 0:
                continue
            aik = a[i][k]
            for j in range(p):
                c[i][j] = (c[i][j] + aik * b[k][j]) % mod
    return c


def pow_mul(a: Matrix, n: int, f0: Matrix, mod: int = MOD) -> Matrix:
    res = f0
    while n:
        if n & 1:
            res = mul(a, res, mod)
        a = mul(a, a, mod)
        n >>= 1
    return res


def mat_pow(a: Matrix, n: int, mod: int = MOD) -> Matrix:
    m = len(a)
    res = [[int(i == j) for j in range(m)] for i in range(m)]
    while n:
        if n & 1:
            res = mul(res, a, mod)
        a = mul(a, a, mod)
        n >>= 1
    return res
