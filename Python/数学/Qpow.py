MOD = 10**9 + 7


def mul(x: int, y: int) -> int:
    return x * y % MOD


def qpow(x: int, y: int) -> int:
    z = 1
    while y > 0:
        if y & 1:
            z = mul(z, x)
        x = mul(x, x)
        y >>= 1
    return z  # 求 x**y % MOD


# 注意：当 MOD 为质数时，(x/y) % MOD = x * y**(MOD-2) % MOD
# 即 y 在模 MOD 意义下的逆元为 qpow(y, MOD - 2)
