MOD = 10**9 + 7


def qpow(x: int, y: int, mod: int = MOD) -> int:
    z = 1
    x %= mod
    while y:
        if y & 1:
            z = z * x % mod
        x = x * x % mod
        y >>= 1
    return z  # 求x**y%MOD


def inv(x: int, mod: int = MOD) -> int:
    # 注意：当MOD为质数时， (x/y)%MOD=(x*(y**(MOD-2)))%MOD，即y在模MOD意义下的逆元为b^{-1} \equiv b^{p-2} mod p
    return qpow(x, mod - 2, mod)
