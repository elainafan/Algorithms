def count(num1: str, num2: str, min_sum: int, max_sum: int) -> int:
    n = len(num2)
    dif = n - len(num1)
    INF = 10**9 + 7
    ma = {}

    def dfs(cnt: int, s: int, limit_low: bool, limit_high: bool) -> int:
        if cnt == n:
            return int(min_sum <= s <= max_sum)
        if s > max_sum:
            return 0
        if not limit_low and not limit_high and (cnt, s) in ma:
            return ma[cnt, s]

        res = 0
        lo = int(num1[cnt - dif]) if limit_low and cnt >= dif else 0
        hi = int(num2[cnt]) if limit_high else 9
        d = lo
        if limit_low and cnt < dif:
            res += dfs(cnt + 1, s, True, False) % INF
            res %= INF
            d = 1
        while d <= hi:
            res += (
                dfs(
                    cnt + 1,
                    s + d,
                    limit_low and d == lo,
                    limit_high and d == hi,
                )
                % INF
            )
            res %= INF
            d += 1

        if not limit_low and not limit_high:
            ma[cnt, s] = res
        return res

    return dfs(0, 0, True, True)


# 表示给定两个整数字符串，表示上下限，以及两个整数表示数位和上下限


# Codeforces 2189D，二进制数位DP+路径输出
# C++ 中这段直接写在 solve() 内；这里将 x、y 作为参数，并返回构造出的 fx、fy。
def solve(x: int, y: int) -> tuple[int, int]:
    vis = [
        [[False] * 3 for _ in range(3)] for _ in range(31)
    ]  # 0一样大，1比x大，2比x小
    ma = [[[2**31 - 1] * 3 for _ in range(3)] for _ in range(31)]
    tem = [(0, 0), (1, 0), (0, 1)]
    pa = [[[(0, 0)] * 3 for _ in range(3)] for _ in range(31)]

    def dfs(t: int, tx: int, ty: int) -> int:
        if t < 0:
            return 0
        if vis[t][tx][ty]:
            return ma[t][tx][ty]

        res = 2**63 - 1
        mask = 1 << t
        for a, b in tem:
            ttx, tty = tx, ty
            xt = x >> t & 1
            yt = y >> t & 1
            temp = 0

            if tx == 0:
                if xt < a:
                    temp += mask
                    ttx = 2
                elif xt > a:
                    temp += mask
                    ttx = 1
            elif tx == 1:
                temp += (xt - a) * mask
            else:
                temp += (a - xt) * mask

            if ty == 0:
                if yt < b:
                    temp += mask
                    tty = 2
                elif yt > b:
                    temp += mask
                    tty = 1
            elif ty == 1:
                temp += (yt - b) * mask
            else:
                temp += (b - yt) * mask

            temp += dfs(t - 1, ttx, tty)
            if temp < res:
                res = temp
                pa[t][tx][ty] = (a, b)

        vis[t][tx][ty] = True
        ma[t][tx][ty] = res
        return res

    dfs(30, 0, 0)
    fx = fy = 0
    tx = ty = 0
    for i in range(30, -1, -1):
        a, b = pa[i][tx][ty]
        if a:
            fx += 1 << i
        if b:
            fy += 1 << i

        xt = x >> i & 1
        yt = y >> i & 1
        if tx == 0:
            if a > xt:
                tx = 2
            elif a < xt:
                tx = 1
        if ty == 0:
            if b > yt:
                ty = 2
            elif b < yt:
                ty = 1

    return fx, fy
