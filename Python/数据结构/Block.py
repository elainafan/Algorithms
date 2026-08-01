from math import isqrt


def solve() -> None:
    n = int(input())
    B = isqrt(n)
    a = list(map(int, input().split()))
    sum = [0] * ((n + B - 1) // B)
    tag = [0] * ((n + B - 1) // B)

    def build() -> None:
        for i in range(n):
            sum[i // B] += a[i]

    # C++ 中两个 add 同名，区间版本最小避名为 add_range
    # 单点加法
    def add(pos: int, val: int) -> None:
        a[pos] += val
        sum[pos // B] += val

    # 区间加法
    def add_range(l: int, r: int, val: int) -> None:
        idl, idr = l // B, r // B
        if idl == idr:
            for i in range(l, r + 1):
                a[i] += val
                sum[idl] += val
        else:
            for i in range(l, (idl + 1) * B):
                a[i] += val
                sum[idl] += val
            for i in range(idl + 1, idr):
                tag[i] += val
                sum[i] += val * B
            for i in range(idr * B, r + 1):
                a[i] += val
                sum[idr] += val

    # 区间查询
    def query(l: int, r: int) -> int:
        res = 0
        idl, idr = l // B, r // B
        if idl == idr:
            for i in range(l, r + 1):
                res += a[i]
        else:
            for i in range(l, (idl + 1) * B):
                res += a[i]
            for i in range(idl + 1, idr):
                res += sum[i]
            for i in range(idr * B, r + 1):
                res += a[i]
        return res

    # C++ 中两个 query 同名，这里将第二个最小避名为 query_tag
    # 带标记区间查询
    def query_tag(l: int, r: int) -> int:
        res = 0
        idl, idr = l // B, r // B
        if idl == idr:
            for i in range(l, r + 1):
                res += a[i] + tag[idl]
        else:
            for i in range(l, (idl + 1) * B):
                res += a[i] + tag[idl]
            for i in range(idl + 1, idr):
                res += sum[i]
            for i in range(idr * B, r + 1):
                res += a[i] + tag[idr]
        return res
