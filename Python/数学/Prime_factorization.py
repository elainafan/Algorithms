MX = 10**5 + 1
lpf = [0] * MX  # 存储每个数的最小素因子，复杂度 O(N log log N)

for i in range(2, MX):
    if lpf[i] == 0:
        for j in range(i, MX, i):
            if lpf[j] == 0:
                lpf[j] = i


# 质因数分解，返回 (素因子, 素因子次幂)，复杂度 O(log N)
def cnt(x: int) -> list[tuple[int, int]]:
    res = []
    while x > 1:
        p = lpf[x]
        e = 1
        x //= p
        while x % p == 0:
            x //= p
            e += 1
        res.append((p, e))
    return res


# 用法：
# factors = cnt(x)  # 要求 1 <= x < MX
