from functools import cache
from typing import Tuple


MOD = 10**9 + 7


def count(num1: str, num2: str, min_sum: int, max_sum: int) -> int:
    # 表示给定两个整数字符串，表示上下限，以及两个整数表示数位和上下限
    n = len(num2)
    dif = n - len(num1)

    @cache
    def dfs(cnt: int, s: int, limit_low: bool, limit_high: bool) -> int:
        if cnt == n:
            return int(min_sum <= s <= max_sum)
        if s > max_sum:
            return 0

        lo = int(num1[cnt - dif]) if limit_low and cnt >= dif else 0
        hi = int(num2[cnt]) if limit_high else 9
        d = lo
        res = 0
        if limit_low and cnt < dif:
            res += dfs(cnt + 1, s, True, False) % MOD
            res %= MOD
            d = 1
        while d <= hi:
            res += dfs(cnt + 1, s + d, limit_low and d == lo, limit_high and d == hi) % MOD
            res %= MOD
            d += 1
        return res

    return dfs(0, 0, True, True)


def min_binary_digit_dp(x: int, y: int, bits: int = 30) -> Tuple[int, int, int]:
    # Codeforces 2189D，二进制数位DP+路径输出
    # 0一样大，1比x大，2比x小
    choices = ((0, 0), (1, 0), (0, 1))
    pa = {}

    @cache
    def dfs(t: int, sx: int, sy: int) -> int:
        if t < 0:
            return 0
        mask = 1 << t
        xb = (x >> t) & 1
        yb = (y >> t) & 1
        res = 10**30
        best_choice = (0, 0, sx, sy)

        for a, b in choices:
            ttx, tty = sx, sy
            temp = 0
            if sx == 0:
                if xb < a:
                    temp += mask
                    ttx = 2
                elif xb > a:
                    temp += mask
                    ttx = 1
            elif sx == 1:
                temp += (xb - a) * mask
            else:
                temp += (a - xb) * mask

            if sy == 0:
                if yb < b:
                    temp += mask
                    tty = 2
                elif yb > b:
                    temp += mask
                    tty = 1
            elif sy == 1:
                temp += (yb - b) * mask
            else:
                temp += (b - yb) * mask

            temp += dfs(t - 1, ttx, tty)
            if temp < res:
                res = temp
                best_choice = (a, b, ttx, tty)

        pa[(t, sx, sy)] = best_choice
        return res

    res = dfs(bits, 0, 0)
    fx = fy = 0
    sx = sy = 0
    for t in range(bits, -1, -1):
        a, b, ttx, tty = pa[(t, sx, sy)]
        if a:
            fx |= 1 << t
        if b:
            fy |= 1 << t
        sx, sy = ttx, tty
    return res, fx, fy
