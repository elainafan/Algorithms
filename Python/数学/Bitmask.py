# 交集 a&b
# 并集 a|b
# 对称差 a^b
# 相对补 a&(~b)
# 包含 a&b=a or a|b=b

# 集合大小 __builtin_popcount/__builtin_popcountll(s)
# 二进制长度 __lg(s）+1
# 集合最大元素 __lg(s)
# 集合最小元素 __builtin_ctz(s) 需要保证s!=0


def iter_bits(mask: int):
    # 遍历集合
    while mask:
        lb = mask & -mask
        yield lb.bit_length() - 1
        mask ^= lb


def iter_submasks(mask: int, include_zero: bool = False):
    # 枚举非空子集
    sub = mask
    while sub:
        # operations
        yield sub
        sub = (sub - 1) & mask
    if include_zero:
        # 枚举所有子集
        yield 0


def iter_supermasks(mask: int, n: int):
    # 枚举超集
    full = 1 << n
    cur = mask
    while cur < full:
        # operations
        yield cur
        cur = (cur + 1) | mask


# 枚举所有集合
def popcount(mask: int) -> int:
    return mask.bit_count()


def lowbit(mask: int) -> int:
    return mask & -mask
