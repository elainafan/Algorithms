from typing import List  # noqa: UP035

MOD = 10**9 + 7
matrix = List[List[int]]  # noqa: UP006


def mul(a: matrix, b: matrix) -> matrix:
    n, m = len(a), len(b[0])
    c = [[0] * m for _ in range(n)]
    for i in range(n):
        for k in range(len(a[i])):
            if a[i][k] == 0:
                continue
            for j in range(m):
                c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % MOD
    return c


def pow_mul(a: matrix, n: int, f0: matrix) -> matrix:
    res = [row[:] for row in f0]
    while n:
        if n & 1:
            res = mul(a, res)
        a = mul(a, a)
        n >>= 1
    return res


# 用法：
# fn = pow_mul(trans, n, f0)
# trans 是转移矩阵，f0 是列向量；返回 trans**n * f0
