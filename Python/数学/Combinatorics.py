from typing import List  # noqa: UP035

MOD = 10**9 + 7
MX = 10**5 + 1

F = [0] * MX  # 预处理阶乘
INV_F = [0] * MX  # 预处理逆元


def mul(x: int, y: int) -> int:
    return x * y % MOD


def qpow(x: int, n: int) -> int:
    res = 1
    while n:
        if n & 1:
            res = res * x % MOD
        x = x * x % MOD
        n >>= 1
    return res


F[0] = 1
for i in range(1, MX):
    F[i] = F[i - 1] * i % MOD

INV_F[MX - 1] = qpow(F[MX - 1], MOD - 2)
for i in range(MX - 1, 0, -1):
    INV_F[i - 1] = INV_F[i] * i % MOD


def comb(n: int, m: int) -> int:
    # 计算 C(n,m)，即从 n 个数中取 m 个数
    if m < 0 or m > n:
        return 0
    return F[n] * INV_F[m] % MOD * INV_F[n - m] % MOD


# 适用于 MX 较小的情况；对应 C++ 文件中的第二份 comb，二选一使用
SMALL_MX = 31
c = [[0] * SMALL_MX for _ in range(SMALL_MX)]
for i in range(SMALL_MX):
    c[i][0] = c[i][i] = 1
    for j in range(1, i):
        c[i][j] = c[i - 1][j - 1] + c[i - 1][j]


def comb_small(n: int, m: int) -> int:
    if n < 0 or m < 0 or m > n:
        return 0
    return c[n][m]


def norm(x: int) -> int:
    return x % MOD


# 下降幂 x^{\underline{k}} = x(x-1)...(x-k+1)
def falling_power(x: int, k: int) -> int:
    res = 1
    for i in range(k):
        res = res * norm(x - i) % MOD
    return res


# 无符号第一类斯特林数 c(n,k)：n 个元素分成 k 个圆排列
# c(n,k) = c(n-1,k-1) + (n-1)c(n-1,k)
def first_unsigned(n: int) -> List[List[int]]:  # noqa: UP006
    s = [[0] * (n + 1) for _ in range(n + 1)]
    s[0][0] = 1
    for i in range(1, n + 1):
        for j in range(1, i + 1):
            s[i][j] = (s[i - 1][j - 1] + (i - 1) * s[i - 1][j]) % MOD
    return s


# 带符号第一类斯特林数 s(n,k)：x^{\underline{n}} = sum s(n,k)x^k
# s(n,k) = s(n-1,k-1) - (n-1)s(n-1,k)
def first_signed(n: int) -> List[List[int]]:  # noqa: UP006
    s = [[0] * (n + 1) for _ in range(n + 1)]
    s[0][0] = 1
    for i in range(1, n + 1):
        for j in range(1, i + 1):
            s[i][j] = norm(s[i - 1][j - 1] - (i - 1) * s[i - 1][j])
    return s


# 第二类斯特林数 S(n,k)：n 个有标号元素分成 k 个非空无标号集合
# S(n,k) = S(n-1,k-1) + kS(n-1,k)
def second(n: int) -> List[List[int]]:  # noqa: UP006
    s = [[0] * (n + 1) for _ in range(n + 1)]
    s[0][0] = 1
    for i in range(1, n + 1):
        for j in range(1, i + 1):
            s[i][j] = (s[i - 1][j - 1] + j * s[i - 1][j]) % MOD
    return s


# 用法：
# comb(n, m)：从 n 个元素中选 m 个，要求 0 <= n < MX
# comb_small(n, m)：小范围杨辉表版本，要求 0 <= n < SMALL_MX
#
# falling_power(x, k) = x(x-1)...(x-k+1)
# 例如 falling_power(n, k) 表示从 n 个不同元素中有顺序选 k 个，即 A(n,k)
#
# s1 = first_unsigned(n)
# s1[i][j] 是无符号第一类斯特林数：i 个元素分成 j 个非空圆排列的方案数
#
# ss1 = first_signed(n)
# ss1[i][j] 是带符号第一类斯特林数，满足
# x^{\underline{i}} = sum(ss1[i][j] * x^j)
#
# s2 = second(n)
# s2[i][j] 是第二类斯特林数：i 个有标号元素分成 j 个非空无标号集合的方案数
#
# 三种斯特林数预处理均为 O(n^2)，查询为 O(1)；falling_power 为 O(k)
