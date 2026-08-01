from typing import List  # noqa: UP035


def zfunc(s: str) -> List[int]:  # noqa: UP006
    n = len(s)
    z = [0] * n
    l = r = 0
    for i in range(1, n):
        if i <= r:
            z[i] = min(z[i - l], r - i + 1)
        while i + z[i] < n and s[z[i]] == s[i + z[i]]:
            l, r = i, i + z[i]
            z[i] += 1
    z[0] = n
    return z


# 用法：
# z = zfunc(s)
# z[i] 表示 s 与 s[i:] 的最长公共前缀长度
