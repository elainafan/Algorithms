from typing import List


def Phi(n: int) -> int:
    res = n
    i = 2
    while i * i <= n:
        if n % i == 0:
            res = res // i * (i - 1)
            while n % i == 0:
                n //= i
        i += 1
    if n > 1:
        res = res // n * (n - 1)
    return res


def precompute_phi(mx: int) -> List[int]:
    phi = list(range(mx))
    for i in range(2, mx):
        if phi[i] == i:
            for j in range(i, mx, i):
                phi[j] = phi[j] // i * (i - 1)
    if mx > 0:
        phi[0] = 0
    if mx > 1:
        phi[1] = 1
    return phi
