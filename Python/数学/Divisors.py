from typing import List


def precompute_divisors(mx: int) -> List[List[int]]:
    divisors = [[] for _ in range(mx)]
    for i in range(1, mx):
        for j in range(i, mx, i):
            divisors[j].append(i)
    return divisors


def get_divisors(n: int) -> List[int]:
    res = []
    tem = []
    i = 1
    while i * i <= n:
        if n % i == 0:
            res.append(i)
            if i * i != n:
                tem.append(n // i)
        i += 1
    return res + tem[::-1]
