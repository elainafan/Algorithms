from typing import List  # noqa: UP035


def kmp(text: str, pattern: str) -> List[int]:  # noqa: UP006
    m = len(pattern)
    pi = [0] * m
    cnt = 0
    for i in range(1, m):
        b = pattern[i]
        while cnt and pattern[cnt] != b:
            cnt = pi[cnt - 1]
        if pattern[cnt] == b:
            cnt += 1
        pi[i] = cnt

    res = []
    cnt = 0
    for i, b in enumerate(text):
        while cnt and pattern[cnt] != b:
            cnt = pi[cnt - 1]
        if pattern[cnt] == b:
            cnt += 1
        if cnt == m:
            res.append(i - m + 1)
            cnt = pi[cnt - 1]
    return res


# C++ 中的第二份 kmp 与上面的函数重名，这里仅作最小改名
def kmp_pi(pattern: str) -> List[int]:  # noqa: UP006
    m = len(pattern)
    cnt = 0
    pi = [0] * m
    for i in range(1, m):
        b = pattern[i]
        while cnt and pattern[cnt] != b:
            cnt = pi[cnt - 1]
        if pattern[cnt] == b:
            cnt += 1
        pi[i] = cnt
    return pi


# 用法：
# pos = kmp(text, pattern)  # pattern 在 text 中所有匹配位置，0-indexed
# pi = kmp_pi(pattern)      # pattern 的前缀函数
# 与 C++ 板子相同，pattern 约定为非空字符串
