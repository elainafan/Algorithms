from typing import List


def prefix_function(s: str) -> List[int]:
    pi = [0] * len(s)
    j = 0
    for i in range(1, len(s)):
        while j and s[i] != s[j]:
            j = pi[j - 1]
        if s[i] == s[j]:
            j += 1
        pi[i] = j
    return pi


def kmp(text: str, pattern: str) -> List[int]:
    if not pattern:
        return list(range(len(text) + 1))
    pi = prefix_function(pattern)
    ans = []
    j = 0
    for i, ch in enumerate(text):
        while j and ch != pattern[j]:
            j = pi[j - 1]
        if ch == pattern[j]:
            j += 1
        if j == len(pattern):
            ans.append(i - len(pattern) + 1)
            j = pi[j - 1]
    return ans
